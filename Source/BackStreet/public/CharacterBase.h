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

//----- ��������Ʈ �Լ� ----------
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

// ------- ��������Ʈ -----------
public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDeleOneParam UpdateAttackState;

// ------- ĳ���� ������Ʈ -------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UChildActorComponent* WeaponActor;

// ------- Character Action �⺻ ------- 
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

// ------ ĳ���� ���� / ����� ---------------
public:
	UFUNCTION(BlueprintCallable)
		void SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION()
		void ResetStatBuffState(bool bIsDebuff, uint8 BuffType);

	//Ư�� Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	UFUNCTION()
		void ClearBuffTimer(bool bIsDebuff, uint8 BuffType);

	UFUNCTION()
		void ClearAllBuffTimer(bool bIsDebuff);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetBuffIsActive(ECharacterBuffType BuffType);

// ----- ĳ���� �ִϸ��̼� -------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		TArray<UAnimMontage*> AttackAnimMontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* DieAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* RollAnimMontage;

// ------ �� �� ĳ���� ������Ƽ ---------------
protected:
	//ĳ������ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStatStruct CharacterStat;

	//ĳ������ ���� ����
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
		FCharacterStateStruct CharacterState;

	UPROPERTY()
		FTimerHandle DelayHandle;
};
