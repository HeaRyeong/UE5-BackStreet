// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "TileBase.generated.h"


UCLASS(Config=Game, defaultconfig)
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
		int32 StageLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelToLoad;


	// ============스테이지 전환============

public:
	UFUNCTION(BlueprintCallable)
		void LoadLevel();
	UFUNCTION(BlueprintCallable)
		void UnLoadLevel();
	UFUNCTION(BlueprintCallable)
		void ClearCheck();

public:
	// ULevelStreaming Instance Ref
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ULevelStreaming* LevelRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AEnemyCharacterBase*> MonsterList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AItemBase*> ItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsClear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsSpawned;


	// ================ 몬스터 스폰 ==================
public:
	UFUNCTION(BlueprintCallable)
		void LoadMonsterAsset();
	UFUNCTION(BlueprintCallable)
		void SpawnMonster();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterSpawnPoints;
	UPROPERTY(Config)
		TArray<FSoftObjectPath> MonsterAssets;

	// =============== 아이템 스폰 ====================

public:
	UFUNCTION(BlueprintCallable)
		void LoadItemAsset();
	UFUNCTION(BlueprintCallable)
		void SpawnItem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemSpawnPoints;
	UPROPERTY(Config)
		TArray<FSoftObjectPath> ItemAssets;

	// ==============		미션	=================
public:
	UFUNCTION(BlueprintCallable)
		void SetMission();
	UFUNCTION(BlueprintCallable)
		void LoadMissionAsset();
	UFUNCTION(BlueprintCallable)
		void SpawnMission();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MissionSpawnPoints;
	UPROPERTY(Config)
		TArray<FSoftObjectPath> MissionAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMissionBase* MissionInfo = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsMainMission;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsBossStage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsMissionClear;


public:
		// 애셋 로딩
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;


};
