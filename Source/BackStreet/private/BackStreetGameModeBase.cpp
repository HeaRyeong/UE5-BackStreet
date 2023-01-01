// Copyright Epic Games, Inc. All Rights Reserved.

#include "../public/BackStreetGameModeBase.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Grid.h"
#include "Tile.h"

ABackStreetGameModeBase::ABackStreetGameModeBase()
{


}

void ABackStreetGameModeBase::InitGame()
{

	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;

	Map = GetWorld()->SpawnActor<AGrid>(AGrid::StaticClass(), spawnLocation, rotator, spawnParams);
	Map->CreateMaze(5, 5);

	CurrTile = Map->GetCurrentTile();
	StageLevel = 1;

}


void ABackStreetGameModeBase::MoveTile(uint8 NextDir)
{
	Map->MoveCurrentTile(NextDir);
	CurrTile = Map->GetCurrentTile();
	//UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::CalculateLoc()] CurrentTransform : %s"), *Map->GetCurrentTile()->GetActorLocation().ToString());
}

void ABackStreetGameModeBase::NextStage(uint8 Dir)
{
	switch ((EDirection)Dir)
	{
	case EDirection::E_UP:
		PreDir = (uint8)(EDirection::E_DOWN);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : DOWN"));
		break;
	case EDirection::E_DOWN:
		PreDir = (uint8)(EDirection::E_UP);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : UP"));
		break;
	case EDirection::E_LEFT:
		PreDir = (uint8)(EDirection::E_RIGHT);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : RIGHT"));
		break;
	case EDirection::E_RIGHT:
		PreDir = (uint8)(EDirection::E_LEFT);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : LEFT"));
		break;
	default:
		break;
	}

	MoveTile(Dir);
}
