// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
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

public:	
	//true : Non-Melee 무기,  false : Melee 무기
	UPROPERTY(EditefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Setup")
		bool bHasProjectile;

public:
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	UFUNCTION()
		void FireProjectile(); 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FProjectileStatStruct ProjectileStat;

	//SoftObjRef로 대체 예정
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

public:
	UFUNCTION()
		void MeleeAttack();

	UFUNCTION()
		void ResetMeleeCombo();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FMeleeWeaponStatStruct MeleeStat;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
};
