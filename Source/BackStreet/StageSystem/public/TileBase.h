// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "../public/StageInfoStruct.h"
#include "../../Global/public/AssetManagerBase.h"
#include "TileBase.generated.h"
#define MaxItemSpawn 3
#define MaxStageType 5

UCLASS()
class BACKSTREET_API ATileBase :public AActor
{
	GENERATED_BODY()

public:
	ATileBase();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

// ===============초기화===============
public:
	UFUNCTION(BlueprintCallable)
		void InitTile(int XPosition, int YPosition);

	UFUNCTION()
		void SelectMap();

	UFUNCTION(BlueprintCallable)
		bool IsVisited();

	UFUNCTION()
		void SetStageType(EStageCategoryInfo type) { StageType = type; }

	UFUNCTION(BlueprintCallable)
		EStageCategoryInfo GetStageType() { return StageType; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 XPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 YPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> Gate; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EStageCategoryInfo StageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ULevelStreaming* LevelRef;
	

public:
	UFUNCTION()
		void SetStage();

	UFUNCTION()
		void SetMission(class UMissionBase* target) { Mission = target; }

	UFUNCTION(BlueprintCallable)
		void MonsterDie(AEnemyCharacterBase* Target);

	UFUNCTION(BlueprintCallable)
		void BindDelegate();

private:
	UFUNCTION()
		void SpawnMonster();

	UFUNCTION()
		void SpawnItem();

	UFUNCTION()
		void SpawnMission();


public:
	UPROPERTY()
		bool bIsVisited;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MissionSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AEnemyCharacterBase*> MonsterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMissionBase* Mission;


public:
	// 스테이지 보상 관련
	UFUNCTION()
		void StageReward();
	UFUNCTION()
		void SetReward();
public:
	UPROPERTY()
		FTimerHandle ClearTimerHandle;

	UPROPERTY()
		int32 ClearTime;

// ---- 참조 -----
public:
	UPROPERTY()
		class ACharacterBase* CharacterRef;
	UPROPERTY()
		class ALevelScriptInGame* InGameScriptRef;
	UPROPERTY()
		class ABackStreetGameModeBase* GameModeRef;

};