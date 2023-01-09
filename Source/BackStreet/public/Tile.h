// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../public/Item.h"
#include "../public/EnemyCharacterBase.h"
#include "Tile.generated.h"


UCLASS()
class BACKSTREET_API ATile :public AActor
{
	GENERATED_BODY()
public:
	// Grid에서 해당 타일의 좌표정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 XPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 YPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> Gate; // Gate 존재 여부

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 StageLevel; // 난이도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelToLoad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ULevelStreaming* LevelRef; // ULevelStreaming Instance Ref


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AEnemyCharacterBase*> MonsterList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AItem*> ItemList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsMainMission;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsBossStage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsClear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsMissionClear;



protected:
	virtual void BeginPlay() override;

public:
	ATile();
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void InitTile(int XPosition, int YPosition);
	UFUNCTION(BlueprintCallable)
		bool IsVisited();
	UFUNCTION(BlueprintCallable)
		void ClearCheck();
	UFUNCTION(BlueprintCallable)
		void SelectMap();
	UFUNCTION(BlueprintCallable)
		void InitMission(bool IsBoss);


public:
	UFUNCTION(BlueprintCallable)
		void LoadLevel();
	UFUNCTION(BlueprintCallable)
		void UnLoadLevel();
	UFUNCTION(BlueprintCallable)
		void SetMission();
};
