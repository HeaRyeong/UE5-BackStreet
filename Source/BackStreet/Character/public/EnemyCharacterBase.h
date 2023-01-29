// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

	
public:
	AEnemyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ----- �⺻ �Լ��� ------
public:
	// �Ҽ� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* TileRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* EnemyStatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EnemyID;

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

// ---- �� ĳ���� Ation ----
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
