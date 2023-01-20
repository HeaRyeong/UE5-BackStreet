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

// ----- 기본 함수들 ------
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

// ---- 디버프 / 버프 -----
	//버프 or 디버프 상태를 지정
	UFUNCTION(BlueprintCallable)
		virtual	bool SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f) override;

	//버프 or 디버프 상태를 초기화한다
	UFUNCTION(BlueprintCallable)
		virtual void ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal) override;

	
// ---- 게임 플레이 ----- 
public:
	// 소속 타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* TileRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* EnemyRankDataTable;

};
