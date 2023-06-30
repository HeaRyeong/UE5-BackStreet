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

	//Weapon이 파괴되었을때 호출할 이벤트
	FDelegateWeaponDestroy WeaponDestroyDelegate;

	//모든 타이머를 해제
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

//------- 기본 프로퍼티, Action -------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		uint8 WeaponID;

	//공격 처리
	virtual void Attack();

	//공격 마무리 처리
	virtual void StopAttack();

//------ 스탯/상태 관련 ---------------------------------
public:
	//Weapon Stat 초기화
	virtual void UpdateWeaponStat(FWeaponStatStruct NewStat);

	UFUNCTION()
		void InitWeapon();

	UFUNCTION()
		void RevertWeaponInfo(FWeaponStatStruct OldWeaponStat, FWeaponStateStruct OldWeaponState);

	UFUNCTION()
		void UpdateDurabilityState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsWeaponRangeType() { return WeaponStat.WeaponType == EWeaponType::E_Shoot || WeaponStat.WeaponType == EWeaponType::E_Throw; }

	//공격 범위를 반환
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
	//Weapon의 종합 Stat
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Stat")
		FWeaponStatStruct WeaponStat;

	//Weapon 상태 정보
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay|Stat")
		FWeaponStateStruct WeaponState;

//-------- 콤보 관련 ------------------------------------
public:
	//콤보 증가
	UFUNCTION(BlueprintCallable)
		void UpdateComboState();

	//Melee Combo 초기화하는 타이머를 등록
	UFUNCTION(BlueprintCallable)
		virtual void SetResetComboTimer();

	//현재 Combo 수를 반환 
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentComboCnt() { return WeaponState.ComboCount; }

//-------- 그 외 (Ref, VFX 등)-------------------------------
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
	//캐릭터 Ref
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		FTimerHandle ComboTimerHandle;
};