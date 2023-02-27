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

	MeleeTrailParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ITEM_NIAGARA_COMPONENT"));
	MeleeTrailParticle->SetupAttachment(WeaponMesh);
	MeleeTrailParticle->bAutoActivate = false;
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

	//무기 스탯이 초기화가 되지 않았다면
	if (WeaponStat.WeaponName == FName(""))
	{
		GamemodeRef->UpdateWeaponStatWithID(this, WeaponID);
	}
	WeaponState.CurrentDurability = WeaponStat.MaxDurability;
}

void AWeaponBase::RevertWeaponInfo(FWeaponStatStruct OldWeaponStat, FWeaponStateStruct OldWeaponState)
{
	if (OldWeaponStat.WeaponName == FName("")) return;
	WeaponStat = OldWeaponStat;
	WeaponState = OldWeaponState;
}

void AWeaponBase::Attack()
{
	//발사체가 있는 무기라면 발사
	if (WeaponStat.bHasProjectile)
	{
		bool result = TryFireProjectile();
		PlayEffectSound(result ? AttackSound : AttackFailSound);
		if (result)	UpdateDurabilityState();
	}
	//근접 공격이 가능한 무기라면 근접 공격 로직 수행
	if (WeaponStat.bCanMeleeAtk)
	{
		PlayEffectSound(AttackSound);
		GetWorldTimerManager().ClearTimer(MeleeComboTimerHandle);
		GetWorldTimerManager().SetTimer(MeleeAtkTimerHandle, this, &AWeaponBase::MeleeAttack, 0.01f, true);
		GetWorldTimerManager().SetTimer(MeleeComboTimerHandle, this, &AWeaponBase::ResetCombo, 1.0f, false, 4.0f - WeaponStat.WeaponAtkSpeedRate);
		MeleeTrailParticle->Activate();
	}
	WeaponState.ComboCount = (WeaponState.ComboCount + 1); //UpdateComboState()? 
}

void AWeaponBase::StopAttack()
{
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
	MeleePrevTracePointList.Empty();
	MeleeLineTraceQueryParams.ClearIgnoredActors();
	MeleeTrailParticle->Deactivate();
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
	SpawnParams.Owner = this; //Projectile의 소유자는 Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = OwnerCharacterRef->GetActorLocation();
	FRotator SpawnRotation = OwnerCharacterRef->GetMesh()->GetComponentRotation();

	//if (WeaponStat.WeaponType == EWeaponType::E_Shoot)
	{
		//SpawnLocation = WeaponMesh->GetSocketLocation(FName("Muzzle"));
	}
	//else
	{
		SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetForwardVector() * 20.0f;
		SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetRightVector() * 50.0f;
	}
	SpawnRotation.Pitch = SpawnRotation.Roll = 0.0f;
	SpawnRotation.Yaw += 90.0f;

	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };
	AProjectileBase* newProjectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));

	if (IsValid(newProjectile))
	{
		newProjectile->SetOwner(this);
		newProjectile->InitProjectile(OwnerCharacterRef);
		newProjectile->ProjectileStat.ProjectileDamage *= WeaponStat.WeaponDamageRate; //버프/디버프로 인해 강화/너프된 값을 반영
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
	if (!WeaponStat.bIsInfiniteAmmo && !OwnerCharacterRef->GetCharacterStat().bInfinite && WeaponState.CurrentAmmoCount == 0)
	{
		if (OwnerCharacterRef->ActorHasTag("Player"))
		{
			GamemodeRef->PrintSystemMessageDelegate.Broadcast(FName(TEXT("탄환이 부족합니다.")), FColor::White);
			UE_LOG(LogTemp, Warning, TEXT("No Ammo"));
		}

		//StopAttack의 ResetActionState로 인해 실행이 되지 않는 현상 방지를 위해
		//타이머를 통해 일정 시간이 지난 후에 Reload를 시도.
		GetWorldTimerManager().SetTimer(AutoReloadTimerHandle, FTimerDelegate::CreateLambda([&]() {
			OwnerCharacterRef->TryReload();
		}), 1.0f, false, AUTO_RELOAD_DELAY_VALUE);
		return false;
	}

	AProjectileBase* newProjectile = CreateProjectile();

	//스폰한 발사체가 Valid 하다면 발사
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
	if (!IsValid(OwnerCharacterRef)) return 200.0f;

	if (WeaponStat.WeaponType == EWeaponType::E_Melee ||
		(!OwnerCharacterRef->GetCharacterStat().bInfinite && WeaponState.CurrentAmmoCount == 0.0f && WeaponState.TotalAmmoCount == 0.0f))
	{
		return 150.0f; //매크로나 const 멤버로 수정하기 
	}
	return 700.0f;
}

void AWeaponBase::UpdateDurabilityState()
{
	if (OwnerCharacterRef->GetCharacterStat().bInfinite || WeaponStat.bInfinite) return;
	if (--WeaponState.CurrentDurability == 0)
	{
		if (IsValid(DestroyEffectParticle))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffectParticle, GetActorLocation(), FRotator()); //파괴 효과
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

	//검로 Trace
	//근접 무기의 각 지점에서 이전 월드 좌표 -> 현재 월드 좌표로 LineTrace를 진행 
	TArray<FVector> currTracePositionList = GetCurrentMeleePointList();
	if (MeleePrevTracePointList.Num() == MAX_LINETRACE_POS_COUNT)
	{
		for (uint8 tracePointIdx = 0; tracePointIdx < MAX_LINETRACE_POS_COUNT; tracePointIdx++)
		{
			const FVector& beginPoint = MeleePrevTracePointList[tracePointIdx];
			const FVector& endPoint = currTracePositionList[tracePointIdx];
			GetWorld()->LineTraceSingleByChannel(hitResult, beginPoint, endPoint, ECollisionChannel::ECC_Camera, MeleeLineTraceQueryParams);

			if (hitResult.bBlockingHit && IsValid(hitResult.GetActor()) //hitResult와 hitActor의 Validity 체크
				&& OwnerCharacterRef->Tags.IsValidIndex(1) //hitActor의 Tags 체크(1)
				&& hitResult.GetActor()->ActorHasTag("Character") //hitActor의 Type 체크
				&& !hitResult.GetActor()->ActorHasTag(OwnerCharacterRef->Tags[1])) //공격자와 피격자의 타입이 같은지 체크
			{
				bIsMeleeTraceSucceed = true;
			//	DrawDebugLine(GetWorld(), beginPoint, endPoint, FColor::Yellow, false, 1.0f, 0, 1.5f);
				break;
			}
			//else
			//	DrawDebugLine(GetWorld(), beginPoint, endPoint, FColor(255, 0, 0), false, 1.0f, 0, 1.5f);
		}
	}
	MeleePrevTracePointList = currTracePositionList;

	//hitResult가 Valid하다면 아래 조건문에서 데미지를 가함
	if (bIsMeleeTraceSucceed)
	{
		// 사운드
		if (HitImpactSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitImpactSound, GetActorLocation());
		}

		//데미지를 주고
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponMeleeDamage * WeaponStat.WeaponDamageRate
										, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);
		(Cast<ACharacterBase>(hitResult.GetActor())->GetBuffManagerRef())->SetDebuffTimer(WeaponStat.DebuffType, OwnerCharacterRef, WeaponStat.DebuffTotalTime, WeaponStat.DebuffVariable);

		//효과 이미터 출력
		if (IsValid(HitEffectParticle))
		{
			FTransform emitterSpawnTransform(FQuat(0.0f), hitResult.Location, FVector(1.5f));
			MeleeLineTraceQueryParams.AddIgnoredActor(hitResult.GetActor());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);
			GamemodeRef->PlayCameraShakeEffect(OwnerCharacterRef->ActorHasTag("Player")? ECameraShakeType::E_Attack : ECameraShakeType::E_Hit, hitResult.Location);
		}
		UpdateDurabilityState();
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

