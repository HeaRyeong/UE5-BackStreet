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
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		UParticleSystem* HitEffectParticle;

	//Weapon�� ���� Stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FWeaponStatStruct WeaponStat;

	//���� ó��
	UFUNCTION()
		void Attack();

	//���� ������ ó��
	UFUNCTION()
		void StopAttack();

	//Weapon Stat �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
		void InitWeaponStat(bool WeaponType, FWeaponStatStruct NewStat);

//------ Projectile ����-------------
public:
	//�߻�ü�� ����
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	//�߻�ü �ʱ�ȭ
	UFUNCTION()
		void FireProjectile(); 

protected:
	//SoftObjRef�� ��ü ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

//-------- Melee ���� ------------
public:
	//Linetrace�� ���� ���� ����
	UFUNCTION()
		void MeleeAttack();

	//���� Combo ���� ��ȯ 
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentMeleeComboCnt() { return MeleeComboCnt; }

	//Melee Combo �ʱ�ȭ
	UFUNCTION()
		void ResetMeleeCombo();

protected:
	//���� ���� Interval
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Setup")
		float MeleeAtkInterval = 0.5f;

	//���� MeleeCombo ��
	UPROPERTY(BlueprintReadOnly)
		int32 MeleeComboCnt = 0;

private:
	//ĳ���� Ref
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		FTimerHandle MeleeAtkTimerHandle;

	UPROPERTY()
		FTimerHandle MeleeComboTimerHandle;

	UPROPERTY()
		float MeleeAtkComboRemainTime = 1.0f;
};
