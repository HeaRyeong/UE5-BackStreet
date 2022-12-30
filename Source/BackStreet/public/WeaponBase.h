// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "WeaponStatStructBase.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"
#define MAX_AMMO_LIMIT_CNT 2000

UCLASS()
class BACKSTREET_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void InitOwnerCharacterRef(class ACharacterBase* NewCharacterRef);

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
		void InitWeaponStat(FWeaponStatStruct NewStat);

//------ Projectile ����-------------
public:
	//�߻�ü�� ����
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	//�߻�ü �ʱ�ȭ �� �߻縦 �õ� 
	UFUNCTION()
		bool TryFireProjectile();

	//�� źâ���� ������, źâ�� ������ ������� �ʴٸ� false ��ȯ
	UFUNCTION(BlueprintCallable)
		bool TryReload();

	//���� źȯ�� ������ ��ȯ - Stat.MaxAmmoCount
	UFUNCTION(BlueprintCallable)
		int32 GetLeftAmmoCount() { return MaxAmmoCount; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetCanReload();

	//źȯ�� ������ ���� (MaxAmmoCount����)
	UFUNCTION(BlueprintCallable)
		void AddAmmo(int32 Count);

	//źâ�� ������ ���� (+= MaxAmmoPerMagazine*Count)
	UFUNCTION()
		void AddMagazine(int32 Count);

	UFUNCTION()
		void SetInfiniteAmmoMode(bool NewMode) { WeaponStat.bIsInfiniteAmmo = NewMode; }

protected:
	//SoftObjRef�� ��ü ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

	//���� źâ�� �ִ� �߻�ü ��
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentAmmoCount = 1;

	//������ �ִ� �ִ� �߻�ü ��
	UPROPERTY(BlueprintReadOnly)
		int32 MaxAmmoCount = 0;

//-------- Melee ���� ------------
public:
	//Linetrace�� ���� ���� ����
	UFUNCTION()
		void MeleeAttack();

	//���� Combo ���� ��ȯ 
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentComboCnt() { return ComboCnt; }

	//Melee Combo �ʱ�ȭ
	UFUNCTION()
		void ResetCombo();

protected:
	//���� MeleeCombo ��
	UPROPERTY(BlueprintReadOnly)
		int32 ComboCnt = 0;

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

	FCollisionQueryParams MeleeLineTraceQueryParams;
};
