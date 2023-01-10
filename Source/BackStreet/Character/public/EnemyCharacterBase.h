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

public:
	// 소속 타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* TileRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* EnemyRankDataTable;

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

};
