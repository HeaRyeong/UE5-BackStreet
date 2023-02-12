// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MissionBase.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Item/public/ItemBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../public/ChapterManagerBase.h"
#include "../public/TileBase.h"
#include "../public/GridBase.h"

// Sets default values
UMissionBase::UMissionBase()
{
 	

}


void UMissionBase::InitMission(ATileBase* tile, int8 type)
{
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
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
			GameModeRef->ChapterManager->RemoveMission(this);
			return true;
		}
		else
			return false;
		break;
	case 2:
		if (MonsterList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			GameModeRef->ChapterManager->RemoveMission(this);
			return true;
		}
		else
			return false;
		break;
	case 3:
		if (MonsterList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			GameModeRef->ChapterManager->RemoveMission(this);
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

void UMissionBase::RemoveItem(class AItemBase* target)
{
	ItemList.Remove(target);
	ClearCheck();
}

void UMissionBase::RemoveMonster(class AEnemyCharacterBase* target)
{
	MonsterList.Remove(target);
	ClearCheck();
}