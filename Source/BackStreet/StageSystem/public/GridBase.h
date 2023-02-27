// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Global/public/BackStreet.h"
#include "../public/DirectionEnumInfo.h"
#include "GridBase.generated.h"
#define ChapterSize 3

UCLASS()
class BACKSTREET_API AGridBase :public AActor
{
	GENERATED_BODY()

		// =====================Maze(Chapter 1°³) »ý¼º=======================
public:
	UFUNCTION(BlueprintCallable)
		void CreateMaze();

	UFUNCTION(BlueprintCallable)
		void RemoveChapter();


private:
	UFUNCTION()
		void RecursiveBacktracking();

	UFUNCTION()
		class ATileBase* GetRandomNeighbourTile(class ATileBase* Tile);	
	
	UFUNCTION(BlueprintCallable)
		void VisitTile(class ATileBase* CurrentTilePara, class ATileBase* NextTilePara);

	UFUNCTION(BlueprintCallable)
		class ATileBase* GetTile(int32 XPosition, int32 YPosition);

public:
	UFUNCTION()
		void AddTime();

	UFUNCTION()
		void PauseTimer();

	UFUNCTION()
		void UnPauseTimer();

	UFUNCTION()
		void ClearTimer();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FTimerHandle GetChapterTimerHandle() { return ChapterTimerHandle; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetChpaterTime() { return ChapterTime; };


public:
	UPROPERTY(VisibleAnywhere)
		FTimerHandle ChapterTimerHandle;

	UPROPERTY(VisibleAnywhere)
		int32 ChapterTime;

private:
	UPROPERTY()
		TArray<class ATileBase*> Tracks;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class ATileBase*> StageArray;

};

