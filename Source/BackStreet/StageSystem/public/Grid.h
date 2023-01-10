// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Global/public/BackStreet.h"
#include "../public/DirectionEnumInfo.h"
#include "Grid.generated.h"

UCLASS()
class BACKSTREET_API AGrid :public AActor
{
	GENERATED_BODY()
public:
	// é�� ũ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Hight;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* CurrentTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATile*> StageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATile*> Tracks; // Maze �������� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATile*> MissionStageArray; // ���� �̼� Class �ļ� Ÿ�� ��� �̼����� �����ҵ�..?

public:
	UFUNCTION(BlueprintCallable)
		void CreateMaze(int32 WidthPara, int32 HightPara);

	UFUNCTION(BlueprintCallable)
		class ATile* GetTile(int32 XPosition, int32 YPosition);

	UFUNCTION(BlueprintCallable)
		class ATile* GetCurrentTile();

	UFUNCTION(BlueprintCallable)
		class ATile* MoveCurrentTile(uint8 Dir);

	UFUNCTION(BlueprintCallable)
		void RemoveChapter();

public:
	UFUNCTION(BlueprintCallable)
		void RecursiveBacktracking();

	UFUNCTION(BlueprintCallable)
		class ATile* GetRandomNeighbourTile(class ATile* Tile);

	UFUNCTION(BlueprintCallable)
		void VisitTile(class ATile* CurrentTilePara, class ATile* NextTilePara);

};
