// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "StageData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateAIContorl);

UCLASS()
class BACKSTREET_API AStageData : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateAIContorl AIOnDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateAIContorl AIOffDelegate;

public:
	UFUNCTION()
		EStageCategoryInfo GetStageType() { return StageType; }
	UFUNCTION(BlueprintCallable)
		bool GetIsVisited() { return bIsVisited; }
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 XPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 YPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector StageLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<bool> GateInfo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		EStageCategoryInfo StageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FName LevelToLoad;

	UPROPERTY(VisibleAnywhere)
		bool bIsVisited;

	UPROPERTY(VisibleAnywhere)
		bool bIsClear;

	UPROPERTY(VisibleAnywhere)
		bool bIsValid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterSpawnPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemSpawnPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> CharacterSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> RewardBoxSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AEnemyCharacterBase*> MonsterList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AItemBase*> ItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AItemBoxBase*> ItemBoxList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ARewardBoxBase* RewardBoxRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ULevelStreaming* LevelRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AGateBase*> GateList;


public:
	UPROPERTY(VisibleAnywhere)
		FTimerHandle StageTimerHandle;

	UPROPERTY(VisibleAnywhere)
		int32 StageTime;
	
};
