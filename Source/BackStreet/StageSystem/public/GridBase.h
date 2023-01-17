// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Global/public/BackStreet.h"
#include "../public/DirectionEnumInfo.h"
#include "GridBase.generated.h"

UCLASS()
class BACKSTREET_API AGridBase :public AActor
{
	GENERATED_BODY()
		// =====================Maze(Chapter 1개) 생성=======================
public:
	UFUNCTION(BlueprintCallable)
		void CreateMaze(int32 WidthPara, int32 HightPara);

	UFUNCTION(BlueprintCallable)
		void RecursiveBacktracking();

	UFUNCTION(BlueprintCallable)
		class ATileBase* GetRandomNeighbourTile(class ATileBase* Tile);

public:
	// 챕터 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Hight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATileBase*> Tracks;

	   // ====================== 타일 이동 ==========================
public:
	UFUNCTION(BlueprintCallable)
		class ATileBase* MoveCurrentTile(uint8 Dir);

	UFUNCTION(BlueprintCallable)
		void VisitTile(class ATileBase* CurrentTilePara, class ATileBase* NextTilePara);

	UFUNCTION(BlueprintCallable)
		class ATileBase* GetTile(int32 XPosition, int32 YPosition);

	UFUNCTION(BlueprintCallable)
		class ATileBase* GetCurrentTile();

	UFUNCTION(BlueprintCallable)
		void RemoveChapter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATileBase*> StageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATileBase*> MissionStageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* CurrentTile;


};
