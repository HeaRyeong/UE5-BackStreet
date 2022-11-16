// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//PlayerMaxHP는 1.0f
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.5f, UIMax = 10.0f))
		float CharacterMaxHP = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.1f, UIMax = 10.0f))
		float CharacterAtkMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 100.0f, UIMax = 1000.0f))
		float CharacterMoveSpeed = 400.0f;
};

USTRUCT(BlueprintType)
struct FPlayerStateStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//현재 구르고 있는지?
	UPROPERTY(BlueprintReadOnly)
		bool bIsRolling = false;

	//현재 공격을 하고 있는지?
	UPROPERTY(BlueprintReadOnly)
		bool bIsAttacking = false;;

	//PlayerMaxHP는 1.0f
	UPROPERTY(BlueprintReadOnly)
		float CharacterCurrHP;
};

UCLASS()
class BACKSTREET_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void InitCharacterState();

	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FPlayerStatStruct NewStat);

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		float TakeDebuffDamage(float DamageAmount);

	void SetDebuffTimer(float Rate, AActor* Causer, FTimerHandle TimerHandle);

	UFUNCTION(BlueprintCallable)
		void SetDebuffTimer(float Rate, AActor* Causer);

	UFUNCTION()
		void ClearDebuffTimer();

	UFUNCTION(BlueprintImplementableEvent) 
		void Die();

protected:
	//SoftObjRef로 대체 예정
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* DieAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* RollAnimMontage;

protected:
	//Action 타이머 핸들
	UPROPERTY()
		FTimerHandle DelayHandle;

	//Debuff는 최대 3개까지 가능
	UPROPERTY()
		FTimerHandle DebuffTimerHandle[3];

	UPROPERTY()
		float DebuffRemainingTime[3];

	//캐릭터의 스탯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		FPlayerStatStruct CharacterStat;
	
	//캐릭터의 현재 상태
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
		FPlayerStateStruct CharacterState;
};
