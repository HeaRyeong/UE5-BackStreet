// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "CharacterBase.h"
#include "CollisionQueryParams.h"
#include "../public/ProjectileBase.h"

FTimerHandle MeleeAtkDelayHandle;
FCollisionQueryParams linetraceCollisionQueryParams;

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
}

void AWeaponBase::Attack()
{
	//발사체가 있는 무기라면 발사
	if (WeaponStat.bHasProjectile) 
	{
		TryFireProjectile();
	}
	GetWorldTimerManager().SetTimer(MeleeAtkTimerHandle, this, &AWeaponBase::MeleeAttack, 0.01f, true);
	GetWorldTimerManager().SetTimer(MeleeComboTimerHandle, this, &AWeaponBase::ResetMeleeCombo, 1.0f, false, 1.0f);
	MeleeAttack();
	MeleeComboCnt = (MeleeComboCnt + 1);
}

void AWeaponBase::StopAttack()
{
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
}

void AWeaponBase::InitWeaponStat(FWeaponStatStruct NewStat)
{
	WeaponStat = NewStat;
}

AProjectileBase* AWeaponBase::CreateProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Projectile의 소유자는 Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f + GetActorRightVector() * 25.0f;
	FRotator SpawnRotation = GetActorRotation();
	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };

	AProjectileBase* newProjectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));
	return newProjectile;
}

bool AWeaponBase::TryLoadMagazine()
{
	if (bInfiniteAmmo || MaxAmmoCount == 0 || CurrentAmmoCount == WeaponStat.MaxAmmoPerMagazine) return false;

	int32 addAmmoCnt = CurrentAmmoCount - MaxAmmoCount % WeaponStat.MaxAmmoPerMagazine;
	CurrentAmmoCount += addAmmoCnt; 
	MaxAmmoCount -= addAmmoCnt;

	return true;
}

void AWeaponBase::AddAmmo(int32 Count)
{
	if (bInfiniteAmmo || MaxAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	MaxAmmoCount = (MaxAmmoCount + Count) % MAX_AMMO_LIMIT_CNT;
}

void AWeaponBase::AddMagazine(int32 Count)
{
	if (bInfiniteAmmo || MaxAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	MaxAmmoCount = (MaxAmmoCount + WeaponStat.MaxAmmoPerMagazine * Count) % MAX_AMMO_LIMIT_CNT;
}

bool AWeaponBase::TryFireProjectile()
{
	if (!bInfiniteAmmo && CurrentAmmoCount == 0) return false;

	AProjectileBase* newProjectile = CreateProjectile();

	//스폰한 발사체가 Valid 하다면 발사
	if (IsValid(newProjectile))
	{
		if(!bInfiniteAmmo) CurrentAmmoCount -= 1;
		newProjectile->ActivateProjectileMovement();
		newProjectile->SetSpawnInstigator(newProjectile->GetInstigator()->GetController());
		return true;
	}
	return false;
}

void AWeaponBase::MeleeAttack()
{	
	FHitResult hitResult;
	FVector StartLocation = WeaponMesh->GetSocketLocation(FName("GrabPoint"));
	FVector EndLocation = WeaponMesh->GetSocketLocation(FName("End"));

	//LineTrace를 통해 hit 된 물체들을 추적
	GetWorld()->LineTraceSingleByChannel(hitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, linetraceCollisionQueryParams);
	
	//hit 되었다면?
	if (hitResult.bBlockingHit && IsValid(OwnerCharacterRef))
	{
		//데미지를 주고
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponDamage, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);

		//효과 이미터 출력
		if (IsValid(HitEffectParticle))
		{
			FTransform emitterSpawnTransform(FQuat(0.0f), FVector(1.0f), hitResult.Location);
			linetraceCollisionQueryParams.AddIgnoredActor(hitResult.GetActor());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);
		}
	}
}

void AWeaponBase::ResetMeleeCombo()
{
	MeleeComboCnt = 0;
	linetraceCollisionQueryParams.ClearIgnoredActors();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::InitOwnerCharacterRef(ACharacterBase* NewCharacterRef)
{
	if (!IsValid(NewCharacterRef)) return;
	OwnerCharacterRef = NewCharacterRef;
}

