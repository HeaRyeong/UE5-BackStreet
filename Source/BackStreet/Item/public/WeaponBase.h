// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"
#define MAX_AMMO_LIMIT_CNT 2000

DECLARE_DELEGATE(FDelegateWeaponDestroy);

UCLASS()
class BACKSTREET_API AWeaponBase : public AActor
{
	GENERATED_BODY()

//------ Global, Component -------------------
public:
	// Sets default values for this actor's properties
	AWeaponBase();

	//Weapon�� �ı��Ǿ����� ȣ���� �̺�Ʈ
	FDelegateWeaponDestroy WeaponDestroyDelegate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* WeaponMesh;

//------- �⺻ ������Ƽ, Action -------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		uint8 WeaponID;

	//���� ó��
	UFUNCTION(BlueprintCallable)
		void Attack();

	//���� ������ ó��
	UFUNCTION(BlueprintCallable)
		void StopAttack();

//------ ����/���� ���� ---------------------------------
public:
	UFUNCTION()
		void InitWeapon();

	UFUNCTION()
		void RevertWeaponInfo(FWeaponStatStruct OldWeaponStat, FWeaponStateStruct OldWeaponState);

	//Weapon Stat �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
		void UpdateWeaponStat(FWeaponStatStruct NewStat);

	//���� ������ ��ȯ
	UFUNCTION(BlueprintCallable)
		float GetAttackRange();

	UFUNCTION()
		void UpdateDurabilityState();

	UFUNCTION(BlueprintCallable)
		FWeaponStatStruct GetWeaponStat() { return WeaponStat; }

	UFUNCTION(BlueprintCallable)
		FWeaponStateStruct GetWeaponState() { return WeaponState; }

protected:
	//Weapon�� ���� Stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FWeaponStatStruct WeaponStat;

	//Weapon ���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FWeaponStateStruct WeaponState;

//------ Projectile ����----------------------------
public:
	//�߻�ü�� ����
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	//�߻�ü �ʱ�ȭ �� �߻縦 �õ� 
	UFUNCTION()
		bool TryFireProjectile();

	//������ �õ�. ���� ���¿� ���� ���� ���θ� ��ȯ
	UFUNCTION(BlueprintCallable)
		bool TryReload();

	//���� źȯ�� ������ ��ȯ - Stat.TotalAmmoCount
	UFUNCTION(BlueprintCallable)
		int32 GetLeftAmmoCount() { return WeaponState.TotalAmmoCount; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetCanReload();

	//źȯ�� ������ ���� (TotalAmmoCount����)
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

//-------- Melee ���� ---------------------------
public:
	//���� Combo ���� ��ȯ 
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentComboCnt() { return WeaponState.ComboCount; }

	//���� ������ ����
	UFUNCTION()
		void MeleeAttack();

	//Melee Combo �ʱ�ȭ
	UFUNCTION()
		void ResetCombo();

	UFUNCTION()
		TArray<FVector> GetCurrentMeleePointList();

private:
	UPROPERTY()
		float MeleeAtkComboRemainTime = 1.0f;

	UPROPERTY()
		TArray<FVector> MeleePrevTracePointList;

	//UPROPERTY()
	FCollisionQueryParams MeleeLineTraceQueryParams;

//-------- �� �� (Ref, VFX ��)-------------------------------
public:
	UFUNCTION(BlueprintCallable)
		void SetOwnerCharacter(class ACharacterBase* NewOwnerCharacterRef);

protected:
	UFUNCTION()
		void PlayEffectSound(class USoundCue* EffectSound);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		class UParticleSystem* HitEffectParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		class UParticleSystem* DestroyEffectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* HitImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* ReloadSound;

private:
	//ĳ���� Ref
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

//------- Timer ���� -----------------------------
	UFUNCTION()
		void ClearAllTimerHandle();

	UPROPERTY()
		FTimerHandle MeleeAtkTimerHandle;

	UPROPERTY()
		FTimerHandle AutoReloadTimerHandle;

	UPROPERTY()
		FTimerHandle MeleeComboTimerHandle;
};