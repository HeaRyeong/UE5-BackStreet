// Fill out your copyright notice in the Description page of Project Settings.
#include "Grid.h"

void AGrid::CreateMaze(int _width, int _hight)
{
	Width = _width;
	Hight = _hight;
	UWorld* world = GetWorld();
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			FActorSpawnParameters spawnParams;
			FRotator rotator;
			FVector spawnLocation = FVector(x * 5000, y * 5000, 0);

			ATile* tile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), spawnLocation, rotator, spawnParams);
			tile->InitTile(x, y);
			GridArray.Push(tile);
		}

	}

	// starting point
	Tracks.Add(GridArray[0]);
	CurrentTile = GridArray[0];
	RecursiveBacktracking();

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			GridArray[y * 5 + x]->CountGate();
		}
	}
}

void AGrid::RecursiveBacktracking()
{
	if (Tracks.Num() == 0) return;

	ATile* currenttile = Tracks[Tracks.Num() - 1];
	ATile* nexttile = GetRandomNeighbourTile(currenttile);

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

ATile* AGrid::GetTile(int x, int y)
{
	if (x >= 0 && x < Width && y >= 0 && y < Hight)
	{
		return (GridArray[(y * Width) + x]);
	}

	return nullptr;
}

ATile* AGrid::GetRandomNeighbourTile(ATile* tile)
{
	ATile* upTile = GetTile(tile->x, tile->y + 1);
	ATile* downTile = GetTile(tile->x, tile->y - 1);
	ATile* leftTile = GetTile(tile->x - 1, tile->y);
	ATile* rightTile = GetTile(tile->x + 1, tile->y);

	TArray<ATile*> neighbourTiles;
	if (upTile != nullptr && !upTile->IsVisited()) neighbourTiles.Add(upTile);
	if (downTile != nullptr && !downTile->IsVisited()) neighbourTiles.Add(downTile);
	if (leftTile != nullptr && !leftTile->IsVisited()) neighbourTiles.Add(leftTile);
	if (rightTile != nullptr && !rightTile->IsVisited()) neighbourTiles.Add(rightTile);

	if (neighbourTiles.Num() == 0)return nullptr;

	int ItemType = FMath::RandRange(0, neighbourTiles.Num() - 1);

	return neighbourTiles[ItemType];
}

ATile* AGrid::MoveCurrentTile(uint8 Dir)
{
	switch ((EDirection)Dir)
	{
	case EDirection::E_UP:
		CurrentTile = GetTile(CurrentTile->x, CurrentTile->y + 1);
		UE_LOG(LogTemp, Log, TEXT("Move to Up"));
		break;
	case EDirection::E_DOWN:
		CurrentTile = GetTile(CurrentTile->x, CurrentTile->y - 1);
		UE_LOG(LogTemp, Log, TEXT("Move to Down"));
		break;
	case EDirection::E_LEFT:
		CurrentTile = GetTile(CurrentTile->x - 1, CurrentTile->y);
		UE_LOG(LogTemp, Log, TEXT("Move to Left"));
		break;
	case EDirection::E_RIGHT:
		CurrentTile = GetTile(CurrentTile->x + 1, CurrentTile->y);
		UE_LOG(LogTemp, Log, TEXT("Move to Right"));
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("Wrong Dir"));
		break;
	}
	return CurrentTile;
}

void AGrid::VisitTile(ATile* _currentTile, ATile* _nextTile)
{
	if (_currentTile->x < _nextTile->x)
	{
		_currentTile->Gate[(uint8)(EDirection::E_RIGHT)] = true;
		_nextTile->Gate[(uint8)(EDirection::E_LEFT)] = true;
	}
	if (_currentTile->x > _nextTile->x)
	{
		_currentTile->Gate[(uint8)(EDirection::E_LEFT)] = true;
		_nextTile->Gate[(uint8)(EDirection::E_RIGHT)] = true;
	}
	if (_currentTile->y < _nextTile->y)
	{
		_currentTile->Gate[(uint8)(EDirection::E_UP)] = true;
		_nextTile->Gate[(uint8)(EDirection::E_DOWN)] = true;
	}
	if (_currentTile->y > _nextTile->y)
	{
		_currentTile->Gate[(uint8)(EDirection::E_DOWN)] = true;
		_nextTile->Gate[(uint8)(EDirection::E_UP)] = true;
	}

	Tracks.Add(_nextTile);
}

ATile* AGrid::GetCurrentTile()
{
	return CurrentTile;
}