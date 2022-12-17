// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "CharacterInfoStructBase.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeleOneParam, bool, NewState);

UCLASS()
class BACKSTREET_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

//----- 오버라이트 함수 ----------
public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack();

	virtual void StopAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ------- 델리게이트 -----------
public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDeleOneParam UpdateAttackState;

// ------- 캐릭터 컴포넌트 -------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UChildActorComponent* WeaponActor;

// ------- Character Action 기본 ------- 
public:
	UFUNCTION()
		void InitCharacterState();

	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FCharacterStatStruct NewStat);

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		float TakeDebuffDamage(float DamageAmount, uint8 DebuffType, AActor* Causer);

	UFUNCTION()
		void TakeHeal(float HealAmount, bool bIsTimerEvent = false, uint8 BuffType = 0);

	UFUNCTION(BlueprintImplementableEvent)
		void Die();

// ------ 캐릭터 버프 / 디버프 ---------------
public:
	UFUNCTION(BlueprintCallable)
		void SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION()
		void ResetStatBuffState(bool bIsDebuff, uint8 BuffType);

	//특정 Debuff의 타이머를 해제한다.
	UFUNCTION()
		void ClearBuffTimer(bool bIsDebuff, uint8 BuffType);

	UFUNCTION()
		void ClearAllBuffTimer(bool bIsDebuff);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetBuffIsActive(ECharacterBuffType BuffType);

// ----- 캐릭터 애니메이션 -------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		TArray<UAnimMontage*> AttackAnimMontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* DieAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* RollAnimMontage;

// ------ 그 외 캐릭터 프로퍼티 ---------------
protected:
	//캐릭터의 스탯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStatStruct CharacterStat;

	//캐릭터의 현재 상태
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
		FCharacterStateStruct CharacterState;

	UPROPERTY()
		FTimerHandle DelayHandle;
};
