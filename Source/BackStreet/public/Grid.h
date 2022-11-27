// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tile.h"
#include "CoreMinimal.h"

/**
 * 
 */
class BACKSTREET_API FGrid
{
public:
	int Width;
	int Hight;
	int CellSize;
	ATile* CurrentTile;
	ATile* GridArray;
	TArray<ATile*> Tracks;

public:
	ATile* GetCurrentTile();
	void CreateMaze(int width, int hight, int cellSize);

private:
	void RecursiveBacktracking();
	ATile* GetTile(int x, int y);
	ATile* GetRandomNeighbourTile(ATile* tile);
	void VisitTile(ATile* currenttile, ATile* nextTile);
};
