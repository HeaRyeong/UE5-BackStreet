// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/WeaponBase.h"
#include "../public/ProjectileBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#define MAX_LINETRACE_POS_COUNT 6

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

	GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AWeaponBase::Attack()
{
	//발사체가 있는 무기라면 발사
	if (WeaponStat.bHasProjectile) 
	{
		TryFireProjectile();
	}
	//근접 공격이 가능한 무기라면 근접 공격 로직 수행
	if (WeaponStat.bCanMeleeAtk)
	{
		GetWorldTimerManager().SetTimer(MeleeAtkTimerHandle, this, &AWeaponBase::MeleeAttack, 0.01f, true);
		GetWorldTimerManager().SetTimer(MeleeComboTimerHandle, this, &AWeaponBase::ResetCombo, 1.5f, false, 1.0f);
	}
	ComboCnt = (ComboCnt + 1);
}

void AWeaponBase::StopAttack()
{
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
	MeleePrevTracePointList.Empty();
	MeleeLineTraceQueryParams.ClearIgnoredActors();
}

void AWeaponBase::UpdateWeaponStat(FWeaponStatStruct NewStat)
{
	WeaponStat = NewStat;
}

AProjectileBase* AWeaponBase::CreateProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Projectile의 소유자는 Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator SpawnRotation = OwnerCharacterRef->GetMesh()->GetComponentRotation();
	FVector SpawnLocation = OwnerCharacterRef->GetActorLocation();

	SpawnRotation.Pitch = 0.0f;
	SpawnRotation.Yaw += 90.0f;
	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetForwardVector() * 20.0f;
	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetMesh()->GetRightVector() * 50.0f;

	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };
	AProjectileBase* newProjectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));

	if (IsValid(newProjectile))
	{
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

	int32 addAmmoCnt = FMath::Min(TotalAmmoCount, WeaponStat.MaxAmmoPerMagazine);
	if (addAmmoCnt + CurrentAmmoCount > WeaponStat.MaxAmmoPerMagazine)
	{
		addAmmoCnt = (WeaponStat.MaxAmmoPerMagazine - CurrentAmmoCount);
	}

	CurrentAmmoCount += addAmmoCnt; 
	TotalAmmoCount -= addAmmoCnt;

	return true;
}

bool AWeaponBase::GetCanReload()
{
	if (WeaponStat.bIsInfiniteAmmo || !WeaponStat.bHasProjectile) return false;
	if (TotalAmmoCount == 0 || CurrentAmmoCount == WeaponStat.MaxAmmoPerMagazine) return false;
	return true;
}

void AWeaponBase::AddAmmo(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || TotalAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	TotalAmmoCount = (TotalAmmoCount + Count) % MAX_AMMO_LIMIT_CNT;
}

void AWeaponBase::AddMagazine(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || TotalAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	TotalAmmoCount = (TotalAmmoCount + WeaponStat.MaxAmmoPerMagazine * Count) % MAX_AMMO_LIMIT_CNT;
}

bool AWeaponBase::TryFireProjectile()
{
	if (CurrentAmmoCount == 0 && !WeaponStat.bIsInfiniteAmmo)
	{
		TryReload();
		return false;
	}

	AProjectileBase* newProjectile = CreateProjectile();

	//스폰한 발사체가 Valid 하다면 발사
	if (IsValid(newProjectile))
	{
		if (!WeaponStat.bIsInfiniteAmmo)
		{
			CurrentAmmoCount -= 1;
		}
		newProjectile->ActivateProjectileMovement();
		return true;
	}
	return false;
}

float AWeaponBase::GetAttackRange()
{
	if (!WeaponStat.bHasProjectile || !WeaponStat.bIsInfiniteAmmo || (CurrentAmmoCount == 0.0f && TotalAmmoCount == 0.0f))
	{
		FVector distVector = WeaponMesh->GetSocketLocation("GrabPoint") - WeaponMesh->GetSocketLocation("End");
		return distVector.Length() * 1.5f;
	}
	return 700.0f;
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
			

			if (hitResult.bBlockingHit && hitResult.GetActor()->ActorHasTag("Character")
				&& !hitResult.GetActor()->ActorHasTag(OwnerCharacterRef->Tags[1]))
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

	//hitResult가 Valid하다면 아래 조건문에서 데미지를 가함
	if (bIsMeleeTraceSucceed)
	{
		//데미지를 주고
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponMeleeDamage * WeaponStat.WeaponDamageRate
										, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);
		Cast<ACharacterBase>(hitResult.GetActor())->SetDebuffTimer(WeaponStat.DebuffType, OwnerCharacterRef, 3.0f, 0.5f);

		//효과 이미터 출력
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
	ComboCnt = 0;
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

void AWeaponBase::InitWeapon(ACharacterBase* NewOwnerCharacterRef)
{
	if (!IsValid(NewOwnerCharacterRef)) return;
	GamemodeRef->UpdateWeaponStatWithID(this, WeaponID);
	OwnerCharacterRef = NewOwnerCharacterRef;
	MeleeLineTraceQueryParams.AddIgnoredActor(OwnerCharacterRef);
}

void AWeaponBase::SetWeaponAmmo(int32 Ammo, int32 TotalAmmo)
{
	CurrentAmmoCount = Ammo;
	TotalAmmoCount = TotalAmmo;
}


int32 AWeaponBase::GetCurrentAmmoCount()
{
	return CurrentAmmoCount;
}

int32 AWeaponBase::GetTotalAmmoCount()
{
	return TotalAmmoCount;
}

void AWeaponBase::PrintWeaponInfo()
{
	UE_LOG(LogTemp, Log, TEXT("CurrentAmmoCount %d , TotalAmmoCount %d"), CurrentAmmoCount, TotalAmmoCount);
}
