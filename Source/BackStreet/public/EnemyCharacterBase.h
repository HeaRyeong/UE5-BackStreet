// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* EnemyRankDataTable;
	
public:
	AEnemyCharacterBase();
	UFUNCTION(BlueprintCallable)
		void InitEnemyStat();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// �Ҽ� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* TileRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABackStreetGameModeBase* GameModeRef;
};
