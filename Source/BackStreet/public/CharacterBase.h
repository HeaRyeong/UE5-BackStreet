// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "CharacterInfoStructBase.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

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

/* Character State Basic */
public:
	UFUNCTION()
		void InitCharacterState();

	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FCharacterStatStruct NewStat);

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
		void Die();

/* Character Debuff */
public:
	UFUNCTION(BlueprintCallable)
		void SetDebuffTimer(float TotalTime, float Damage, ECharacterDebuffType DebuffType, AActor* Causer);

	UFUNCTION()
		void ResetNonDamageDebuff(ECharacterDebuffType DebuffType, float ResetValue=0.0f);

	UFUNCTION()
		float TakeDebuffDamage(float DamageAmount, ECharacterDebuffType DebuffType, AActor* Causer);

	//특정 Debuff의 타이머를 해제한다.
	UFUNCTION()
		void ClearDebuffTimer(ECharacterDebuffType DebuffType);

	UFUNCTION()
		void ClearAllDebuffTimer();


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
	//캐릭터의 스탯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStatStruct CharacterStat;

	//캐릭터의 현재 상태
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
		FCharacterStateStruct CharacterState;

protected:
	//Action 타이머 핸들
	UPROPERTY()
		FTimerHandle DelayHandle;

	UPROPERTY()
		float BuffRemainingTime[5];

	UPROPERTY()
		FTimerHandle BuffTimerHandle[5];

	UPROPERTY()
		float DebuffRemainingTime[10];

	UPROPERTY()
		FTimerHandle DebuffTimerHandle[10];
};
