// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "CollisionQueryParams.h"
#include "WeaponStatStructBase.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class BACKSTREET_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//------ Global -------------------
public:	
	//true : Non-Melee 무기,  false : Melee 무기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Setup")
		bool bHasProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		UParticleSystem* HitEffectParticle;

	UFUNCTION()
		void Attack();

//------ Projectile 무기-------------
public:
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	UFUNCTION()
		void FireProjectile(); 

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetAttackAnimIndex() { return bHasProjectile ? 0 : GetCurrentMeleeComboCnt(); }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FProjectileStatStruct ProjectileStat;

	//SoftObjRef로 대체 예정
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

//-------- Melee 무기 ------------
public:
	UFUNCTION()
		void MeleeAttack();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentMeleeComboCnt() { return MeleeComboCnt; }

	UFUNCTION()
		void ResetMeleeCombo();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FMeleeWeaponStatStruct MeleeStat;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		TArray<UAnimMontage*> MeleeAtkAnimMontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Setup")
		float MeleeAtkInterval = 0.5f;

	UPROPERTY(BlueprintReadOnly)
		int32 MeleeComboCnt = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Setup")
		int32 MeleeMaxComboCnt = 3;

private:
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;
};
