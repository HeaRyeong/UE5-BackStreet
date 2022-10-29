// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStateStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//=--------Character Stats--------
	//PlayerMaxHP는 1.0f
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float CharacterMaxHP = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float CharacterAtkMultiplier = 1.0f;

	//UPROPERTY(EditDefaultsOnly)
	//	float Ch

	//=---------Curr State -------------
	//현재 구르고 있는지?
	UPROPERTY(BlueprintReadOnly)
		bool bIsRolling = false;

	//현재 공격을 하고 있는지?
	UPROPERTY(BlueprintReadOnly)
		bool bIsAttacking = false;;

	//PlayerMaxHP는 1.0f
	UPROPERTY(BlueprintReadOnly)
		float PlayerCurrHP;

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
	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat();

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
						, AController* EventInstigator, AActor* DamageCauser) override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		FPlayerStateStruct CharacterState;
};
