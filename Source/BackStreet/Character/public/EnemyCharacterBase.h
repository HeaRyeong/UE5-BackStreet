// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

DECLARE_DELEGATE_OneParam(FDelegateEnemyDeath, class AEnemyCharacterBase*);

UCLASS()
class BACKSTREET_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

// ------ Global, Component ------------
public:
	AEnemyCharacterBase();
	
	//�� Death �̺�Ʈ
	FDelegateEnemyDeath EnemyDeathDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|UI")
		class UWidgetComponent* FloatingHpBar;

// ----- �⺻ �Լ��� ------
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
		int32 EnemyID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		int32 DefaultWeaponID;

public:
	UFUNCTION(BlueprintCallable)
		void InitEnemyStat();

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
		virtual void TryAttack() override;

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
		virtual void StopAttack() override;

	UFUNCTION()
		virtual void Die() override;

protected:
	UFUNCTION()
		void SetDefaultWeapon();

	UFUNCTION()
		void SetDefaultStat();

	UFUNCTION(BlueprintImplementableEvent)
		void InitFloatingHpWidget();

// ---- �� ĳ���� Action ----
public:
	UFUNCTION(BlueprintCallable)
		void Turn(float Angle);

// ---- ����� / ���� -----
	//���� or ����� ���¸� ����
	UFUNCTION(BlueprintCallable)
		virtual	bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f) override;

	//���� or ����� ���¸� �ʱ�ȭ�Ѵ�
	UFUNCTION(BlueprintCallable)
		virtual void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal) override;
};
