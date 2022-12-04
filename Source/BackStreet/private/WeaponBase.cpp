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

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacterRef = Cast<ACharacterBase>(GetOwner());
}

void AWeaponBase::Attack()
{
	if (bHasProjectile)
	{

	}
	else
	{

	}
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

void AWeaponBase::FireProjectile()
{
	AProjectileBase* newProjectile = CreateProjectile();

	if (IsValid(newProjectile))
	{
		newProjectile->ActivateProjectileMovement();
		newProjectile->SetSpawnInstigator(newProjectile->GetInstigator()->GetController());
	}
}

void AWeaponBase::MeleeAttack()
{
	if (MeleeComboCnt <= MeleeAtkAnimMontageArray.Num() 
		&& IsValid(MeleeAtkAnimMontageArray[MeleeComboCnt]))
	{
		MeleeComboCnt = (MeleeComboCnt + 1) % MeleeMaxComboCnt;
	}
	FHitResult hitResult;
	FVector StartLocation = Mesh->GetSocketLocation(FName("GrabPoint"));
	FVector EndLocation = Mesh->GetSocketLocation(FName("End"));

	GetWorld()->LineTraceSingleByChannel(hitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, linetraceCollisionQueryParams);
	if (hitResult.bBlockingHit && IsValid(HitEffectParticle))
	{
		FTransform emitterSpawnTransform(FQuat(0.0f), FVector(1.0f), hitResult.Location);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);
		linetraceCollisionQueryParams.AddIgnoredActor(hitResult.GetActor());
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), MeleeStat.WeaponDamage, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);
	}
}

void AWeaponBase::ResetMeleeCombo()
{

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

