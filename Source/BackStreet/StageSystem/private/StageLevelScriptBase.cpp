// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/StageLevelScriptBase.h"
#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"

void AStageLevelScriptBase::BeginPlay()
{
	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());

	LoadSpawnPoint();
}

void AStageLevelScriptBase::LoadSpawnPoint()
{
	/*if (!IsValid(GamemodeRef) || !IsValid(GamemodeRef->CurrentTile)) return;

	ATileBase* currentTileRef = GamemodeRef->CurrentTile;
	TArray<AActor*>& monsterSpawnPointList = currentTileRef->MonsterSpawnPoints;
	TArray<AActor*>& itemSpawnPointList = currentTileRef->ItemSpawnPoints;
	TArray<AActor*>& missionSpawnPointList = currentTileRef->MissionSpawnPoints;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MonsterSpawnPoint"), monsterSpawnPointList);
	currentTileRef->LoadMonster();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("ItemSpawnPoint"), itemSpawnPointList);
	currentTileRef->LoadItem();

	if(currentTileRef->bIsMainMission && currentTileRef->MissionInfo != nullptr)
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MissionSpawnPoint"), missionSpawnPointList);
		currentTileRef->LoadMissionAsset();
	}*/
}