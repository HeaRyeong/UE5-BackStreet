// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/RangedWeaponBase.h"
#include "../public/ProjectileBase.h"
#include "../public/WeaponBase.h"
#include "../../Global/public/DebuffManager.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#define AUTO_RELOAD_DELAY_VALUE 0.1

void ARangedWeaponBase::Attack()
{
	Super::Attack();

	if (WeaponStat.bHasProjectile)
	{
		bool result = TryFireProjectile();
		PlayEffectSound(result ? AttackSound : AttackFailSound);
		if (result)	UpdateDurabilityState();
	}
}

void ARangedWeaponBase::StopAttack()
{
	Super::StopAttack();
}

void ARangedWeaponBase::ClearAllTimerHandle()
{
	Super::ClearAllTimerHandle();
}

AProjectileBase* ARangedWeaponBase::CreateProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Projectile�� �����ڴ� Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = OwnerCharacterRef->GetActorLocation();
	FRotator SpawnRotation = OwnerCharacterRef->GetMesh()->GetComponentRotation();

	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetForwardVector() * 20.0f;
	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetRightVector() * 50.0f;

	SpawnRotation.Pitch = SpawnRotation.Roll = 0.0f;
	SpawnRotation.Yaw += 90.0f;

	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };
	AProjectileBase* newProjectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));

	if (IsValid(newProjectile))
	{
		newProjectile->SetOwner(this);
		newProjectile->InitProjectile(OwnerCharacterRef);
		newProjectile->ProjectileStat.ProjectileDamage *= WeaponStat.WeaponDamageRate; //����/������� ���� ��ȭ/������ ���� �ݿ�
		newProjectile->ProjectileStat.ProjectileSpeed *= WeaponStat.WeaponAtkSpeedRate;
		return newProjectile;
	}
	return nullptr;
}

bool ARangedWeaponBase::TryReload()
{
	if (!GetCanReload()) return false;

	int32 addAmmoCnt = FMath::Min(WeaponState.TotalAmmoCount, WeaponStat.MaxAmmoPerMagazine);
	if (addAmmoCnt + WeaponState.CurrentAmmoCount > WeaponStat.MaxAmmoPerMagazine)
	{
		addAmmoCnt = (WeaponStat.MaxAmmoPerMagazine - WeaponState.CurrentAmmoCount);
	}
	WeaponState.CurrentAmmoCount += addAmmoCnt;
	WeaponState.TotalAmmoCount -= addAmmoCnt;

	return true;
}

bool ARangedWeaponBase::GetCanReload()
{
	if (WeaponStat.bIsInfiniteAmmo || !WeaponStat.bHasProjectile) return false;
	if (WeaponState.TotalAmmoCount == 0 || WeaponState.CurrentAmmoCount == WeaponStat.MaxAmmoPerMagazine) return false;
	return true;
}

void ARangedWeaponBase::AddAmmo(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || WeaponState.TotalAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	WeaponState.TotalAmmoCount = (WeaponState.TotalAmmoCount + Count) % MAX_AMMO_LIMIT_CNT;
}

void ARangedWeaponBase::AddMagazine(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || WeaponState.TotalAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	WeaponState.TotalAmmoCount = (WeaponState.TotalAmmoCount + WeaponStat.MaxAmmoPerMagazine * Count) % MAX_AMMO_LIMIT_CNT;
}

bool ARangedWeaponBase::TryFireProjectile()
{
	if (!IsValid(OwnerCharacterRef)) return false;
	if (!WeaponStat.bIsInfiniteAmmo && !OwnerCharacterRef->GetCharacterStat().bInfinite && WeaponState.CurrentAmmoCount == 0)
	{
		if (OwnerCharacterRef->ActorHasTag("Player"))
		{
			GamemodeRef->PrintSystemMessageDelegate.Broadcast(FName(TEXT("źȯ�� �����մϴ�.")), FColor::White);
			UE_LOG(LogTemp, Warning, TEXT("No Ammo"));
		}

		//StopAttack�� ResetActionState�� ���� ������ ���� �ʴ� ���� ������ ����
		//Ÿ�̸Ӹ� ���� ���� �ð��� ���� �Ŀ� Reload�� �õ�.
		GetWorldTimerManager().SetTimer(AutoReloadTimerHandle, FTimerDelegate::CreateLambda([&]() {
			OwnerCharacterRef->TryReload();
		}), 1.0f, false, AUTO_RELOAD_DELAY_VALUE);
		return false;
	}

	const int32 fireProjectileCnt = FMath::Min(WeaponState.CurrentAmmoCount, OwnerCharacterRef->GetCharacterStat().MaxProjectileCount);
	for (int idx = 1; idx <= fireProjectileCnt; idx++)
	{
		FTimerHandle delayHandle;
		GetWorld()->GetTimerManager().SetTimer(delayHandle, FTimerDelegate::CreateLambda([&]() {
			AProjectileBase* newProjectile = CreateProjectile();
		//������ �߻�ü�� Valid �ϴٸ� �߻�
		if (IsValid(newProjectile))
		{
			if (!WeaponStat.bIsInfiniteAmmo && !OwnerCharacterRef->GetCharacterStat().bInfinite)
			{
				WeaponState.CurrentAmmoCount -= 1;
			}
			newProjectile->ActivateProjectileMovement();
		}
			}), 0.1f * (float)idx, false);
	}
	return true;
}