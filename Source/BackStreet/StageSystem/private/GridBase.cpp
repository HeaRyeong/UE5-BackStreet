// Fill out your copyright notice in the Description page of Project Settings.
#include "../public/GridBase.h"
#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"

void AGridBase::CreateMaze()
{
	UWorld* world = GetWorld();
	for (int y = 0; y < ChapterSize; y++)
	{
		for (int x = 0; x < ChapterSize; x++)
		{
			FActorSpawnParameters spawnParams;
			FRotator rotator;
			FVector spawnLocation = FVector(x * 5000, y * 5000, 0);

			ATileBase* tile = GetWorld()->SpawnActor<ATileBase>(ATileBase::StaticClass(), spawnLocation, rotator, spawnParams);
			tile->InitTile(x, y);
			StageArray.Push(tile);
		}

	}

	// starting point
	Tracks.Add(StageArray[0]);
	RecursiveBacktracking();

	ChapterTime = 0;

	Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ClearResourceDelegate.AddDynamic(this, &AGridBase::ClearTimer);
	GetWorldTimerManager().SetTimer(ChapterTimerHandle, this, &AGridBase::AddTime, 1.0f, true);
	GetWorldTimerManager().PauseTimer(ChapterTimerHandle);

}

void AGridBase::RemoveChapter()
{
	for (ATileBase* tile : StageArray)
	{
		if (!IsValid(tile)) continue;
		tile->Destroy();
	}
	ClearTimer();
	Destroy();
}


void AGridBase::RecursiveBacktracking()
{
	if (Tracks.Num() == 0) return;

	ATileBase* currenttile = Tracks[Tracks.Num() - 1];
	ATileBase* nexttile = GetRandomNeighbourTile(currenttile);

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

ATileBase* AGridBase::GetRandomNeighbourTile(ATileBase* Tile)
{
	ATileBase* upTile = GetTile(Tile->XPos + 1, Tile->YPos);
	ATileBase* downTile = GetTile(Tile->XPos - 1, Tile->YPos);
	ATileBase* leftTile = GetTile(Tile->XPos, Tile->YPos - 1);
	ATileBase* rightTile = GetTile(Tile->XPos, Tile->YPos + 1);

	TArray<ATileBase*> neighbourTiles;
	if (upTile != nullptr && !upTile->IsVisited()) neighbourTiles.Add(upTile);
	if (downTile != nullptr && !downTile->IsVisited()) neighbourTiles.Add(downTile);
	if (leftTile != nullptr && !leftTile->IsVisited()) neighbourTiles.Add(leftTile);
	if (rightTile != nullptr && !rightTile->IsVisited()) neighbourTiles.Add(rightTile);

	if (neighbourTiles.Num() == 0)return nullptr;

	int ItemType = FMath::RandRange(0, neighbourTiles.Num() - 1);

	return neighbourTiles[ItemType];
}

void AGridBase::VisitTile(ATileBase* CurrentTilePara, ATileBase* NextTilePara)
{
	if (CurrentTilePara->XPos < NextTilePara->XPos)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_UP)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_DOWN)] = true;
	}
	if (CurrentTilePara->XPos > NextTilePara->XPos)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_DOWN)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_UP)] = true;
	}
	if (CurrentTilePara->YPos < NextTilePara->YPos)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_RIGHT)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_LEFT)] = true;
	}
	if (CurrentTilePara->YPos > NextTilePara->YPos)
	{
		CurrentTilePara->Gate[(uint8)(EDirection::E_LEFT)] = true;
		NextTilePara->Gate[(uint8)(EDirection::E_RIGHT)] = true;
	}

	Tracks.Add(NextTilePara);
}

ATileBase* AGridBase::GetTile(int32 XPosition, int32 YPosition)
{
	if (XPosition >= 0 && XPosition < ChapterSize && YPosition >= 0 && YPosition < ChapterSize)
	{
		return (StageArray[(YPosition * ChapterSize) + XPosition]);
	}

	return nullptr;
}


void AGridBase::AddTime()
{
	ChapterTime++;
}


void AGridBase::PauseTimer()
{
	UE_LOG(LogTemp, Log, TEXT("Call Chapter Timer Pause"));
	GetWorldTimerManager().PauseTimer(ChapterTimerHandle);
}

void AGridBase::UnPauseTimer()
{
	UE_LOG(LogTemp, Log, TEXT("Call Chapter Timer UnPause"));
	GetWorldTimerManager().UnPauseTimer(ChapterTimerHandle);
}

void AGridBase::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	//GetWorldTimerManager().ClearTimer(StageTimerHandle);
}

