// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tile.h"
#include "CoreMinimal.h"
#include "DirectionEnumInfo.h"
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
		ATile* CurrentTile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ATile*> StageArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ATile*> Tracks; // Maze �������� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ATile*> MissionStageArray; // ���� �̼� Class �ļ� Ÿ�� ��� �̼����� �����ҵ�..?

public:
	UFUNCTION(BlueprintCallable)
		void CreateMaze(int32 WidthPara, int32 HightPara);
	UFUNCTION(BlueprintCallable)
		ATile* GetTile(int32 XPos, int32 YPos);
	UFUNCTION(BlueprintCallable)
		ATile* GetCurrentTile();
	UFUNCTION(BlueprintCallable)
		ATile* MoveCurrentTile(uint8 Dir);
	UFUNCTION(BlueprintCallable)
		void RemoveChapter();

public:
	UFUNCTION(BlueprintCallable)
		void RecursiveBacktracking();
	UFUNCTION(BlueprintCallable)
		ATile* GetRandomNeighbourTile(ATile* Tile);
	UFUNCTION(BlueprintCallable)
		void VisitTile(ATile* CurrentTilePara, ATile* NextTilePara);

};
