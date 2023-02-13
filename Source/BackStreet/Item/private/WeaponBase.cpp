// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/WeaponBase.h"
#include "../public/ProjectileBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Character/public/CharacterBuffManager.h"
#include "Components/AudioComponent.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#define MAX_LINETRACE_POS_COUNT 6
#define AUTO_RELOAD_DELAY_VALUE 0.1

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON_MESH"));
	WeaponMesh->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	InitWeapon();
}

void AWeaponBase::InitWeapon()
{
	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//���� ������ �ʱ�ȭ�� ���� �ʾҴٸ�
	if (WeaponStat.WeaponName == FName(""))
	{
		GamemodeRef->UpdateWeaponStatWithID(this, WeaponID);
	}
	WeaponState.CurrentDurability = WeaponStat.MaxDurability;
	UE_LOG(LogTemp, Warning, TEXT("Init) Current Durability : %d"), WeaponState.CurrentDurability);
}

void AWeaponBase::RevertWeaponInfo(FWeaponStatStruct OldWeaponStat, FWeaponStateStruct OldWeaponState)
{
	if (OldWeaponStat.WeaponName == FName("")) return;
	WeaponStat = OldWeaponStat;
	WeaponState = OldWeaponState;
}

void AWeaponBase::Attack()
{
	//�߻�ü�� �ִ� ������ �߻�
	if (WeaponStat.bHasProjectile)
	{
		TryFireProjectile();
		
		if (OwnerCharacterRef->GetCharacterState().CharacterActionState == ECharacterActionType::E_Reload)
		{
			//PlayEffectSound(AttackSound); �� ��Ƽ� �Ҹ�?
		}
		else
		{
			PlayEffectSound(AttackSound);
		}
	}
	//���� ������ ������ ������ ���� ���� ���� ����
	if (WeaponStat.bCanMeleeAtk)
	{
		PlayEffectSound(AttackSound);
		GetWorldTimerManager().SetTimer(MeleeAtkTimerHandle, this, &AWeaponBase::MeleeAttack, 0.01f, true);
		GetWorldTimerManager().SetTimer(MeleeComboTimerHandle, this, &AWeaponBase::ResetCombo, 1.5f, false, 1.0f);
	}
	UpdateDurabilityState();
	UE_LOG(LogTemp, Warning, TEXT("ATK) Current Durability : %d"), WeaponState.CurrentDurability);
	WeaponState.ComboCount = (WeaponState.ComboCount + 1); //UpdateComboState()? 
}

void AWeaponBase::StopAttack()
{
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
	MeleePrevTracePointList.Empty();
	MeleeLineTraceQueryParams.ClearIgnoredActors();
}

void AWeaponBase::UpdateWeaponStat(FWeaponStatStruct NewStat)
{
	if (NewStat.MaxDurability == 0) return;
	WeaponStat = NewStat;
}

void AWeaponBase::SetOwnerCharacter(ACharacterBase* NewOwnerCharacterRef)
{
	if (!IsValid(NewOwnerCharacterRef)) return;
	OwnerCharacterRef = NewOwnerCharacterRef;
	MeleeLineTraceQueryParams.AddIgnoredActor(OwnerCharacterRef);
}

void AWeaponBase::PlayEffectSound(USoundCue* EffectSound)
{
	if (EffectSound == nullptr || !IsValid(OwnerCharacterRef) || !OwnerCharacterRef->ActorHasTag("Player")) return;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EffectSound, GetActorLocation());
}

void AWeaponBase::ClearAllTimerHandle()
{
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
	GetWorldTimerManager().ClearTimer(AutoReloadTimerHandle);
	GetWorldTimerManager().ClearTimer(MeleeComboTimerHandle);
}

AProjectileBase* AWeaponBase::CreateProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Projectile�� �����ڴ� Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator SpawnRotation = OwnerCharacterRef->GetMesh()->GetComponentRotation();
	FVector SpawnLocation = OwnerCharacterRef->GetActorLocation();

	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Yaw += 90.0f;

	if (WeaponStat.WeaponType == EWeaponType::E_Shoot)
	{
		SpawnLocation = WeaponMesh->GetSocketLocation(FName("Muzzle"));
	}
	else
	{
		SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetForwardVector() * 20.0f;
		SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetRightVector() * 50.0f;
	}

	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };
	AProjectileBase* newProjectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));

	if (IsValid(newProjectile))
	{
		newProjectile->InitProjectile(OwnerCharacterRef);
		newProjectile->ProjectileStat.ProjectileDamage *= WeaponStat.WeaponDamageRate; //����/������� ���� ��ȭ/������ ���� �ݿ�
		newProjectile->ProjectileStat.ProjectileSpeed *= WeaponStat.WeaponAtkSpeedRate;
		return newProjectile;
	}

	return nullptr;
}

bool AWeaponBase::TryReload()
{
	if (!GetCanReload()) return false;

	int32 addAmmoCnt = FMath::Min(WeaponState.TotalAmmoCount, WeaponStat.MaxAmmoPerMagazine);
	if (addAmmoCnt + WeaponState.CurrentAmmoCount > WeaponStat.MaxAmmoPerMagazine)
	{
		addAmmoCnt = (WeaponStat.MaxAmmoPerMagazine - WeaponState.CurrentAmmoCount);
	}
	PlayEffectSound(ReloadSound);
	WeaponState.CurrentAmmoCount += addAmmoCnt;
	WeaponState.TotalAmmoCount -= addAmmoCnt;

	return true;
}

bool AWeaponBase::GetCanReload()
{
	if (WeaponStat.bIsInfiniteAmmo || !WeaponStat.bHasProjectile) return false;
	if (WeaponState.TotalAmmoCount == 0 || WeaponState.CurrentAmmoCount == WeaponStat.MaxAmmoPerMagazine) return false;
	return true;
}

void AWeaponBase::AddAmmo(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || WeaponState.TotalAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	WeaponState.TotalAmmoCount = (WeaponState.TotalAmmoCount + Count) % MAX_AMMO_LIMIT_CNT;
}

void AWeaponBase::AddMagazine(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || WeaponState.TotalAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	WeaponState.TotalAmmoCount = (WeaponState.TotalAmmoCount + WeaponStat.MaxAmmoPerMagazine * Count) % MAX_AMMO_LIMIT_CNT;
}

bool AWeaponBase::TryFireProjectile()
{
	if (!IsValid(OwnerCharacterRef)) return false;
	if (WeaponState.CurrentAmmoCount == 0 && !WeaponStat.bIsInfiniteAmmo)
	{
		//StopAttack�� ResetActionState�� ���� ������ ���� �ʴ� ���� ������ ����
		//Ÿ�̸Ӹ� ���� ���� �ð��� ���� �Ŀ� Reload�� �õ�.
		GetWorldTimerManager().SetTimer(AutoReloadTimerHandle, FTimerDelegate::CreateLambda([&]() {
			OwnerCharacterRef->TryReload();
		}), 1.0f, false, AUTO_RELOAD_DELAY_VALUE);
		return false;
	}

	AProjectileBase* newProjectile = CreateProjectile();

	//������ �߻�ü�� Valid �ϴٸ� �߻�
	if (IsValid(newProjectile))
	{
		if (!WeaponStat.bIsInfiniteAmmo && !OwnerCharacterRef->GetCharacterStat().bInfinite)
		{
			WeaponState.CurrentAmmoCount -= 1;
		}
		newProjectile->ActivateProjectileMovement();
		return true;
	}
	return false;
}

float AWeaponBase::GetAttackRange()
{
	if (!WeaponStat.bHasProjectile || !WeaponStat.bIsInfiniteAmmo
		|| (WeaponState.CurrentAmmoCount == 0.0f && WeaponState.TotalAmmoCount == 0.0f))
	{
		return 200.0f; //��ũ�γ� const ����� �����ϱ� 
	}
	return 700.0f;
}

void AWeaponBase::UpdateDurabilityState()
{
	if (OwnerCharacterRef->GetCharacterStat().bInfinite) return;
	if (--WeaponState.CurrentDurability == 0)
	{
		if (IsValid(DestroyEffectParticle))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffectParticle, GetActorLocation(), FRotator()); //�ı� ȿ��
		}
		ClearAllTimerHandle();
		OwnerCharacterRef->StopAttack();
		WeaponDestroyDelegate.ExecuteIfBound();
	}
}

void AWeaponBase::MeleeAttack()
{
	FHitResult hitResult;
	bool bIsMeleeTraceSucceed = false;
	FVector StartLocation = WeaponMesh->GetSocketLocation(FName("GrabPoint"));
	FVector EndLocation = WeaponMesh->GetSocketLocation(FName("End"));

	//�˷� Trace
	//���� ������ �� �������� ���� ���� ��ǥ -> ���� ���� ��ǥ�� LineTrace�� ���� 
	TArray<FVector> currTracePositionList = GetCurrentMeleePointList();
	if (MeleePrevTracePointList.Num() == MAX_LINETRACE_POS_COUNT)
	{
		for (uint8 tracePointIdx = 0; tracePointIdx < MAX_LINETRACE_POS_COUNT; tracePointIdx++)
		{
			const FVector& beginPoint = MeleePrevTracePointList[tracePointIdx];
			const FVector& endPoint = currTracePositionList[tracePointIdx];
			GetWorld()->LineTraceSingleByChannel(hitResult, beginPoint, endPoint, ECollisionChannel::ECC_Camera, MeleeLineTraceQueryParams);

			if (hitResult.bBlockingHit && IsValid(hitResult.GetActor()) //hitResult�� hitActor�� Validity üũ
				&& OwnerCharacterRef->Tags.IsValidIndex(1) //hitActor�� Tags üũ(1)
				&& hitResult.GetActor()->ActorHasTag("Character") //hitActor�� Type üũ
				&& !hitResult.GetActor()->ActorHasTag(OwnerCharacterRef->Tags[1])) //�����ڿ� �ǰ����� Ÿ���� ������ üũ
			{
				bIsMeleeTraceSucceed = true;
				DrawDebugLine(GetWorld(), beginPoint, endPoint, FColor::Yellow, false, 1.0f, 0, 1.5f);
				break;
			}
			else
				DrawDebugLine(GetWorld(), beginPoint, endPoint, FColor(255, 0, 0), false, 1.0f, 0, 1.5f);
		}
	}
	MeleePrevTracePointList = currTracePositionList;

	//hitResult�� Valid�ϴٸ� �Ʒ� ���ǹ����� �������� ����
	if (bIsMeleeTraceSucceed)
	{
		// ����
		if (HitImpactSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitImpactSound, GetActorLocation());
		}

		//�������� �ְ�
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponMeleeDamage * WeaponStat.WeaponDamageRate
										, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);
		(Cast<ACharacterBase>(hitResult.GetActor())->GetBuffManagerRef())->SetDebuffTimer(WeaponStat.DebuffType, OwnerCharacterRef, 3.0f, 0.5f);

		//ȿ�� �̹��� ���
		if (IsValid(HitEffectParticle))
		{
			FTransform emitterSpawnTransform(FQuat(0.0f), hitResult.Location, FVector(1.5f));
			MeleeLineTraceQueryParams.AddIgnoredActor(hitResult.GetActor());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);
		}
	}
}

void AWeaponBase::ResetCombo()
{
	WeaponState.ComboCount = 0;
}

TArray<FVector> AWeaponBase::GetCurrentMeleePointList()
{
	TArray<FVector> resultPosList;

	resultPosList.Add(WeaponMesh->GetSocketLocation("GrabPoint"));
	resultPosList.Add(WeaponMesh->GetSocketLocation("End"));
	for (uint8 positionIdx = 1; positionIdx < MAX_LINETRACE_POS_COUNT - 1; positionIdx++)
	{
		FVector direction = resultPosList[1] - resultPosList[0];

		resultPosList.Add(resultPosList[0] + direction / MAX_LINETRACE_POS_COUNT * positionIdx);
	}
	return resultPosList;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

