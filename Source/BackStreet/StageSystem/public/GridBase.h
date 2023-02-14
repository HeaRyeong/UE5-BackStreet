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

		// =====================Maze(Chapter 1��) ����=======================
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


private:
	UPROPERTY()
		TArray<class ATileBase*> Tracks;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATileBase*> StageArray;

};

//UCLASS()
//class BACKSTREET_API AGridBase :public AActor
//{
//	GENERATED_BODY()
//		// =====================Maze(Chapter 1��) ����=======================
//public:
//	UFUNCTION(BlueprintCallable)
//		void CreateMaze(int32 WidthPara, int32 HightPara);
//
//	UFUNCTION(BlueprintCallable)
//		void RecursiveBacktracking();
//
//	UFUNCTION(BlueprintCallable)
//		class ATileBase* GetRandomNeighbourTile(class ATileBase* Tile);
//
//public:
//	// é�� ũ��
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		int32 Width;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		int32 Hight;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		TArray<class ATileBase*> Tracks;
//
//	// ====================== Ÿ�� �̵� ==========================
//public:
//	UFUNCTION(BlueprintCallable)
//		class ATileBase* MoveCurrentTile(uint8 Dir);
//
//	UFUNCTION(BlueprintCallable)
//		void VisitTile(class ATileBase* CurrentTilePara, class ATileBase* NextTilePara);
//
//	UFUNCTION(BlueprintCallable)
//		class ATileBase* GetTile(int32 XPosition, int32 YPosition);
//
//	UFUNCTION(BlueprintCallable)
//		class ATileBase* GetCurrentTile();
//
//	UFUNCTION(BlueprintCallable)
//		void RemoveChapter();
//
//	UFUNCTION(BlueprintCallable)
//		void CheckChapterClear();
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		bool bIsChapterClear = false;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		TArray<class ATileBase*> StageArray;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		TArray<class ATileBase*> MissionStageArray;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		TArray<class UMissionBase*> Missions;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		class ATileBase* CurrentTile;
//
//
//};