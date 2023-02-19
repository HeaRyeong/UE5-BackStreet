// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "../public/StageInfoStruct.h"
#include "../../Global/public/AssetManagerBase.h"
#include "TileBase.generated.h"
#define MAX_ITEM_SPAWN 3
#define MaxStageType 5

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateNoParam);


UCLASS()
class BACKSTREET_API ATileBase :public AActor
{
	GENERATED_BODY()

//------ Delegate -----------------------
public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateNoParam StartTileDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateNoParam FinishTileDelegate;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 XPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 YPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<bool> Gate; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EStageCategoryInfo StageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FName LevelToLoad;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ULevelStreaming* LevelRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ALevelScriptBase* ScriptRef;

public:
	UFUNCTION()
		void SetStage();

	UFUNCTION()
		void SetMission(class UMissionBase* target) { Mission = target; }

	UFUNCTION(BlueprintCallable)
		void MonsterDie(AEnemyCharacterBase* Target);

	UFUNCTION(BlueprintCallable)
		void ActivateAI();

	UFUNCTION(BlueprintCallable)
		void DeactivateAI();


private:
	UFUNCTION()
		void SpawnMonster();

	UFUNCTION()
		void SpawnItem();

	UFUNCTION()
		void SpawnMission();

	UFUNCTION(BlueprintCallable)
		void BindDelegate();


public:

	UFUNCTION(BlueprintCallable)
		TArray<class AEnemyCharacterBase*> GetMonsterList() { return MonsterList; };

public:
	UPROPERTY()
		bool bIsVisited;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterSpawnPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemSpawnPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> MissionSpawnPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AActor*> CharacterSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AEnemyCharacterBase*> MonsterList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMissionBase* Mission;


public:
	// 스테이지 보상 관련
	UFUNCTION()
		void StageReward();
	UFUNCTION()
		void SetReward();
public:
	UPROPERTY(VisibleAnywhere)
		FTimerHandle ClearTimerHandle;

	UPROPERTY(VisibleAnywhere)
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