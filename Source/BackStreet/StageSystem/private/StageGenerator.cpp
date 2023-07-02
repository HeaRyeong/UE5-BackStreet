// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/StageGenerator.h"
#include "../public/ChapterManagerBase.h"
#include "../public/ResourceManager.h"
#include "../../StageSystem/public/StageData.h"

TArray<class AStageData*> UStageGenerator::CreateMaze()
{
	Stages.Empty();
	UWorld* world = GetWorld();
	for (int y = 0; y < ChapterSize; y++)
	{
		for (int x = 0; x < ChapterSize; x++)
		{
			FVector spawnLocation = FVector(x * 5000, y * 5000, 0);
			FActorSpawnParameters spawnParams;
			FRotator rotator;

			AStageData* target = NewObject<AStageData>(this->GetOuter());
			InitStageData(spawnLocation, y, x, target);

			Stages.Push(target);
		
		}

	}	
	
	Tracks.Add(Stages[0]);
	RecursiveBacktracking();

	int32 bossIdx = FMath::RandRange(0, Stages.Num() - 1);
	Stages[bossIdx]->StageType = EStageCategoryInfo::E_Boss;

	UE_LOG(LogTemp, Log, TEXT("Complete Create Maze"));


	return Stages;

	//ChapterTime = 0;

	//Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ClearResourceDelegate.AddDynamic(this, &AGridBase::ClearTimer);
	//GetWorldTimerManager().SetTimer(ChapterTimerHandle, this, &AGridBase::AddTime, 1.0f, true);
	//GetWorldTimerManager().PauseTimer(ChapterTimerHandle);

}

void UStageGenerator::InitStageData(FVector Vector, int32 Ypos, int32 Xpos, class AStageData* Target)
{
	Target->XPos = Xpos;
	Target->YPos = Ypos;
	Target->StageLocation = Vector;
	Target->bIsValid = true;
	Target->bIsVisited = Target->bIsClear = false;
	Target->GateInfo.Add(false); // UP
	Target->GateInfo.Add(false); // DOWN
	Target->GateInfo.Add(false); // LEFT
	Target->GateInfo.Add(false); // RIGHT

	Target->StageType = EStageCategoryInfo::E_Normal;
	Target->LevelToLoad = Cast<AChapterManagerBase>(this->GetOuter())->GetResourceManager()->GetRandomMap();

}


void UStageGenerator::RecursiveBacktracking()
{
	if (Tracks.Num() == 0) return;

	class AStageData* currenttile = Tracks[Tracks.Num() - 1];
	class AStageData* nexttile = GetRandomNeighbourTile(currenttile);

	if (nexttile->bIsValid)
	{
		VisitTile(currenttile, nexttile);

	}
	else
	{
		Tracks.Pop();

	}

	RecursiveBacktracking();
}

class AStageData* UStageGenerator::GetRandomNeighbourTile(class AStageData* Tile)
{
	AStageData* upTile = GetTile(Tile->XPos + 1, Tile->YPos);
	AStageData* downTile = GetTile(Tile->XPos - 1, Tile->YPos);
	AStageData* leftTile = GetTile(Tile->XPos, Tile->YPos - 1);
	AStageData* rightTile = GetTile(Tile->XPos, Tile->YPos + 1);

	TArray<AStageData*> neighbourTiles;
	if (upTile!=nullptr&&upTile->bIsValid && !(upTile->GateInfo[static_cast<int>(EDirection::E_UP)] || upTile->GateInfo[static_cast<int>(EDirection::E_DOWN)] || upTile->GateInfo[static_cast<int>(EDirection::E_LEFT)] || upTile->GateInfo[static_cast<int>(EDirection::E_RIGHT)])) neighbourTiles.Add(upTile);
	if (downTile!=nullptr&&downTile->bIsValid && !(downTile->GateInfo[static_cast<int>(EDirection::E_UP)] || downTile->GateInfo[static_cast<int>(EDirection::E_DOWN)] || downTile->GateInfo[static_cast<int>(EDirection::E_LEFT)] || downTile->GateInfo[static_cast<int>(EDirection::E_RIGHT)])) neighbourTiles.Add(downTile);
	if (leftTile!=nullptr&&leftTile->bIsValid && !(leftTile->GateInfo[static_cast<int>(EDirection::E_UP)] || leftTile->GateInfo[static_cast<int>(EDirection::E_DOWN)] || leftTile->GateInfo[static_cast<int>(EDirection::E_LEFT)] || leftTile->GateInfo[static_cast<int>(EDirection::E_RIGHT)])) neighbourTiles.Add(leftTile);
	if (rightTile!=nullptr&&rightTile->bIsValid && !(rightTile->GateInfo[static_cast<int>(EDirection::E_UP)] || rightTile->GateInfo[static_cast<int>(EDirection::E_DOWN)] || rightTile->GateInfo[static_cast<int>(EDirection::E_LEFT)] || rightTile->GateInfo[static_cast<int>(EDirection::E_RIGHT)])) neighbourTiles.Add(rightTile);

	if (neighbourTiles.Num() == 0)
	{
		AStageData* target = NewObject<AStageData>();
		target->bIsValid = false;
		return target;
	}

	int ItemType = FMath::RandRange(0, neighbourTiles.Num() - 1);

	return neighbourTiles[ItemType];
}

void UStageGenerator::VisitTile(class AStageData* CurrentTilePara, class AStageData* NextTilePara)
{
	if (CurrentTilePara->XPos < NextTilePara->XPos)
	{
		CurrentTilePara->GateInfo[(uint8)(EDirection::E_UP)] = true;
		NextTilePara->GateInfo[(uint8)(EDirection::E_DOWN)] = true;
	}
	if (CurrentTilePara->XPos > NextTilePara->XPos)
	{
		CurrentTilePara->GateInfo[(uint8)(EDirection::E_DOWN)] = true;
		NextTilePara->GateInfo[(uint8)(EDirection::E_UP)] = true;
	}
	if (CurrentTilePara->YPos < NextTilePara->YPos)
	{
		CurrentTilePara->GateInfo[(uint8)(EDirection::E_RIGHT)] = true;
		NextTilePara->GateInfo[(uint8)(EDirection::E_LEFT)] = true;
	}
	if (CurrentTilePara->YPos > NextTilePara->YPos)
	{
		CurrentTilePara->GateInfo[(uint8)(EDirection::E_LEFT)] = true;
		NextTilePara->GateInfo[(uint8)(EDirection::E_RIGHT)] = true;
	}

	Tracks.Add(NextTilePara);
}

class AStageData* UStageGenerator::GetTile(int32 XPosition, int32 YPosition)
{
	if (XPosition >= 0 && XPosition < ChapterSize && YPosition >= 0 && YPosition < ChapterSize)
	{
		return (Stages[(YPosition * ChapterSize) + XPosition]);
	}
	return nullptr;
	
}

