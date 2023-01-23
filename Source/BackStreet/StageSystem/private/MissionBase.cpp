// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MissionBase.h"
#include "../public/TileBase.h"
#include "../public/GridBase.h"

// Sets default values
UMissionBase::UMissionBase()
{
 	

}


void UMissionBase::InitMission()
{
	UE_LOG(LogTemp, Log, TEXT("Call InitMission "));
	Type = FMath::RandRange(1, 3);
}

void UMissionBase::ClearCheck()
{
	switch (Type)
	{
	case 1:
		if (ItemList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			Tile->Chapter->Missions.Remove(this);
			Tile->Chapter->CheckChapterClear();
		}
		else
		{

		}
		break;
	case 2:
		if (MonsterList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			Tile->Chapter->Missions.Remove(this);
			Tile->Chapter->CheckChapterClear();
		}
		else
		{

		}
		break;
	case 3:
		if (MonsterList.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Mission Clear "));
			Tile->Chapter->Missions.Remove(this);
			Tile->Chapter->CheckChapterClear();
		}
		else
		{

		}
		break;
	default:
		break;
	}
}