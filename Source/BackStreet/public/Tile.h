// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "EnemyCharacterBase.h"
#include "Tile.generated.h"


UCLASS()
class BACKSTREET_API ATile :public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelToLoad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ULevelStreaming* LevelRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AEnemyCharacterBase*> MonsterList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AItem*> ItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int StageLevel;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> Gate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsClear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int GateCnt;


protected:
	virtual void BeginPlay() override;

public:
	ATile();
	UFUNCTION(BlueprintCallable)
		void InitTile(int width, int hight);
	UFUNCTION(BlueprintCallable)
		bool IsVisited();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void CountGate();
	UFUNCTION(BlueprintCallable)
		void ClearCheck();

};
