// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "../public/StageInfoStruct.h"
#include "../../Global/public/AssetManagerBase.h"
#include "TileBase.generated.h"
#define MaxItemSpawn 3

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

	UFUNCTION(BlueprintCallable)
		bool IsVisited();

	UFUNCTION(BlueprintCallable)
		void SelectMap();

	UFUNCTION(BlueprintCallable)
		void InitMission(bool IsBoss);

public:
	// Grid에서 해당 타일의 좌표정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 XPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 YPos;
	// Gate 존재 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> Gate; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EStageCategoryInfo StageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelToLoad;


// ============스테이지 전환============
public:
	UFUNCTION(BlueprintCallable)
		void LoadLevel();
	UFUNCTION(BlueprintCallable)
		void UnLoadLevel();

public:
	// ULevelStreaming Instance Ref
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ULevelStreaming* LevelRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AEnemyCharacterBase*> MonsterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsClear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSpawned;


// ================ 몬스터 관련 ==================
public:
	UFUNCTION(BlueprintCallable)
		void LoadMonster();

	UFUNCTION(BlueprintCallable)
		void MonsterDie(AEnemyCharacterBase* Target);

	UFUNCTION(BlueprintCallable)
		void BindDelegate();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterSpawnPoints;


// =============== 아이템 스폰 ====================
public:
	UFUNCTION(BlueprintCallable)
		void LoadItem();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemSpawnPoints;


// ==============  미션	=================
public:
	UFUNCTION(BlueprintCallable)
		void LoadMissionAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MissionSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMissionBase* MissionInfo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsMainMission;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsBossStage;

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
		class AGridBase* Chapter;

	UPROPERTY()
		class ACharacterBase* CharacterRef;
	
	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		 AAssetManagerBase* AssetDataManagerRef;

};
