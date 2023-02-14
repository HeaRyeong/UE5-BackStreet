// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MissionBase.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Item/public/ItemBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../public/ChapterManagerBase.h"
#include "../public/TileBase.h"
#include "../public/GridBase.h"
#include "../public/ALevelScriptInGame.h"

// Sets default values
UMissionBase::UMissionBase()
{
 	

}


void UMissionBase::InitMission(ATileBase* tile, int8 type)
{
	//GameModeRef = Cast<AALevelScriptInGame>(UGameplayStatics::GetGameMode(GetWorld()));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
	UE_LOG(LogTemp, Log, TEXT("Call InitMission "));
	BelongTile = tile;
	Type = type;
}

bool UMissionBase::ClearCheck()
{
	switch (Type)
	{
	case 1:
		if (ItemList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			InGameScriptRef->ChapterManager->RemoveMission(this);
			return true;
		}
		else
			return false;
		break;
	case 2:
		if (MonsterList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			InGameScriptRef->ChapterManager->RemoveMission(this);
			return true;
		}
		else
			return false;
		break;
	case 3:
		if (MonsterList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			InGameScriptRef->ChapterManager->RemoveMission(this);
			return true;
		}
		else
			return false;
		break;
	default:
			return false;
		break;
	}
}

bool UMissionBase::TryAddMissionItem(AItemBase* target)
{
	if (ItemList.Num() >= MAX_MISSION_ITEM_COUNT) return false;
	ItemList.Add(target);
	return true;
}

bool UMissionBase::TryRemoveMissionItem(class AItemBase* target)
{
	if (ItemList.Find(target) == INDEX_NONE) return false;
	ItemList.Remove(target);
	return ClearCheck();
}

bool UMissionBase::TryRemoveMonster(class AEnemyCharacterBase* target)
{
	if (MonsterList.Find(target) == INDEX_NONE) return false;
	MonsterList.Remove(target);
	return ClearCheck();
}