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

	//��� Ÿ�̸Ӹ� ����
	virtual void ClearAllTimerHandle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UNiagaraComponent* MeleeTrailParticle;

//------- �⺻ ������Ƽ, Action -------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		uint8 WeaponID;

	//���� ó��
	virtual void Attack();

	//���� ������ ó��
	virtual void StopAttack();

//------ ����/���� ���� ---------------------------------
public:
	//Weapon Stat �ʱ�ȭ
	virtual void UpdateWeaponStat(FWeaponStatStruct NewStat);

	UFUNCTION()
		void InitWeapon();

	UFUNCTION()
		void RevertWeaponInfo(FWeaponStatStruct OldWeaponStat, FWeaponStateStruct OldWeaponState);

	UFUNCTION()
		void UpdateDurabilityState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsWeaponRangeType() { return WeaponStat.WeaponType == EWeaponType::E_Shoot || WeaponStat.WeaponType == EWeaponType::E_Throw; }

	//���� ������ ��ȯ
	virtual float GetAttackRange();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FWeaponStatStruct GetWeaponStat() { return WeaponStat; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FWeaponStateStruct GetWeaponState() { return WeaponState; }

	UFUNCTION()
		void SetWeaponStat(FWeaponStatStruct NewStat) { WeaponStat = NewStat; }

	UFUNCTION()
		void SetWeaponState(FWeaponStateStruct NewState) { WeaponState = NewState; }

protected:
	//Weapon�� ���� Stat
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Stat")
		FWeaponStatStruct WeaponStat;

	//Weapon ���� ����
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay|Stat")
		FWeaponStateStruct WeaponState;

//-------- �޺� ���� ------------------------------------
public:
	//�޺� ����
	UFUNCTION(BlueprintCallable)
		void UpdateComboState();

	//Melee Combo �ʱ�ȭ�ϴ� Ÿ�̸Ӹ� ���
	UFUNCTION(BlueprintCallable)
		virtual void SetResetComboTimer();

	//���� Combo ���� ��ȯ 
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentComboCnt() { return WeaponState.ComboCount; }

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
		class USoundCue* AttackFailSound;

protected:
	//ĳ���� Ref
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		FTimerHandle ComboTimerHandle;
};