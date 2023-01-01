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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Hight;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATile* CurrentTile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ATile*> GridArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ATile*> Tracks;

public:
	UFUNCTION(BlueprintCallable)
		ATile* GetCurrentTile();
	UFUNCTION(BlueprintCallable)
		void CreateMaze(int _width, int _hight);
	UFUNCTION(BlueprintCallable)
		ATile* MoveCurrentTile(uint8 Dir);

public:
	UFUNCTION(BlueprintCallable)
		void RecursiveBacktracking();
	UFUNCTION(BlueprintCallable)
		ATile* GetTile(int x, int y);
	UFUNCTION(BlueprintCallable)
		ATile* GetRandomNeighbourTile(ATile* tile);
	UFUNCTION(BlueprintCallable)
		void VisitTile(ATile* _currenttile, ATile* _nextTile);


};
