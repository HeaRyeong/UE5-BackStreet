// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

void FGrid::CreateMaze(int width, int hight, int cellSize)
{
	Width = width;
	Hight = hight;
	CellSize = cellSize;

	GridArray = new FTile[width * hight];

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < hight; y++)
		{
			GridArray[(y * width) + x].InitTile(x, y);

		}

	}
	// starting point
	Tracks.Add(&(GridArray[0]));
	CurrentTile = &(GridArray[0]);
	RecursiveBacktracking();
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < hight; y++)
		{
			GridArray[(y * width) + x].GateCount();

		}

	}
}

void FGrid::RecursiveBacktracking()
{
	if (Tracks.Num() == 0) return;

	FTile* currenttile = Tracks[Tracks.Num() - 1];
	FTile* nexttile = GetRandomNeighbourTile(currenttile);

	if (nexttile != nullptr)
	{
		VisitTile(currenttile, nexttile);

	}
	else
	{
		Tracks.Pop();

	}

	RecursiveBacktracking();
}

FTile* FGrid::GetTile(int x, int y)
{
	if (x >= 0 && x < Width && y >= 0 && y < Hight)
	{
		return &(GridArray[(y * Width) + x]);
	}

	return nullptr;
}

FTile* FGrid::GetRandomNeighbourTile(FTile* tile)
{
	FTile* upTile = GetTile(tile->x, tile->y + 1);
	FTile* downTile = GetTile(tile->x, tile->y - 1);
	FTile* leftTile = GetTile(tile->x - 1, tile->y);
	FTile* rightTile = GetTile(tile->x + 1, tile->y);

	TArray<FTile*> neighbourTiles;
	if (upTile != nullptr && !upTile->IsVisited()) neighbourTiles.Add(upTile);
	if (downTile != nullptr && !downTile->IsVisited()) neighbourTiles.Add(downTile);
	if (leftTile != nullptr && !leftTile->IsVisited()) neighbourTiles.Add(leftTile);
	if (rightTile != nullptr && !rightTile->IsVisited()) neighbourTiles.Add(rightTile);

	if (neighbourTiles.Num() == 0)return nullptr;

	srand(time(NULL));
	return neighbourTiles[rand() % neighbourTiles.Num()];
}

void FGrid::VisitTile(FTile* currentTile, FTile* nextTile)
{
	if (currentTile->x < nextTile->x)
	{
		currentTile->right = true;
		nextTile->left = true;
	}
	if (currentTile->x > nextTile->x)
	{
		currentTile->left = true;
		nextTile->right = true;
	}
	if (currentTile->y < nextTile->y)
	{
		currentTile->up = true;
		nextTile->down = true;
	}
	if (currentTile->y > nextTile->y)
	{
		currentTile->down = true;
		nextTile->up = true;
	}

	Tracks.Add(nextTile);
}

FTile* FGrid::GetCurrentTile()
{
	return CurrentTile;
}