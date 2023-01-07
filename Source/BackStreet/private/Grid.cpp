// Fill out your copyright notice in the Description page of Project Settings.
#include "Grid.h"

void AGrid::CreateMaze(int32 WidthPara, int32 HightPara)
{
	Width = WidthPara;
	Hight = HightPara;
	UWorld* world = GetWorld();
	for (int y = 0; y < HightPara; y++)
	{
		for (int x = 0; x < WidthPara; x++)
		{
			FActorSpawnParameters spawnParams;
			FRotator rotator;
			FVector spawnLocation = FVector(x * 5000, y * 5000, 0);

			ATile* tile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), spawnLocation, rotator, spawnParams);
			tile->InitTile(x, y);
			StageArray.Push(tile);
		}

	}

	// starting point
	Tracks.Add(StageArray[0]);
	CurrentTile = StageArray[0];
	RecursiveBacktracking();

	// 미션 타일 뽑기 ( 메인 3 , 보스 1 )
	TArray<int32> MissionTileidxList;

	for (int i = 0; i < Width * Hight; i++)
	{
		MissionTileidxList.Add(i);
	}

	for (int i = 0; i < 100; i++)
	{
		int32 SelectidxA = FMath::RandRange(0, Width*Hight -1);
		int32 SelectidxB = FMath::RandRange(0, Width * Hight - 1);
		int32 Temp;
	
		Temp = MissionTileidxList[SelectidxA];
		MissionTileidxList[SelectidxA] = MissionTileidxList[SelectidxB];
		MissionTileidxList[SelectidxB] = Temp;
	}


	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[Grid::CreateMaze()] BossMissionTildidx : %d"), MissionTileidxList[i]);
			MissionStageArray.Add(StageArray[MissionTileidxList[0]]);
			StageArray[MissionTileidxList[0]]->InitMission(true);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[Grid::CreateMaze()] MissionTildidx : %d"), MissionTileidxList[i]);
			MissionStageArray.Add(StageArray[MissionTileidxList[i]]);
			StageArray[MissionTileidxList[i]]->InitMission(false);
		}
	}


	for (ATile* Tile : StageArray)
		Tile->SelectMap();
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

ATile* AGrid::GetTile(int32 XPos, int32 YPos)
{
	if (XPos >= 0 && XPos < Width && YPos >= 0 && YPos < Hight)
	{
		return (StageArray[(YPos * Width) + XPos]);
	}

	return nullptr;
}

ATile* AGrid::GetRandomNeighbourTile(ATile* Tile)
{
	ATile* upTile = GetTile(Tile->x, Tile->y + 1);
	ATile* downTile = GetTile(Tile->x, Tile->y - 1);
	ATile* leftTile = GetTile(Tile->x - 1, Tile->y);
	ATile* rightTile = GetTile(Tile->x + 1, Tile->y);

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

void AGrid::VisitTile(ATile* CurrentTilePara, ATile* NextTilePara)
{
	if (CurrentTilePara->x < NextTilePara->x)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_RIGHT)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_LEFT)] = true;
	}
	if (CurrentTilePara->x > NextTilePara->x)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_LEFT)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_RIGHT)] = true;
	}
	if (CurrentTilePara->y < NextTilePara->y)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_UP)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_DOWN)] = true;
	}
	if (CurrentTilePara->y > NextTilePara->y)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_DOWN)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_UP)] = true;
	}

	Tracks.Add(NextTilePara);
}

ATile* AGrid::GetCurrentTile()
{
	return CurrentTile;
}

void AGrid::RemoveChapter()
{
	for (ATile* tile : StageArray)
	{
		tile->Destroy();
	}
	Destroy();
}