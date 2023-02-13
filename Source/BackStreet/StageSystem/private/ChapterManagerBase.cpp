// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ChapterManagerBase.h"
#include "../public/StageManagerBase.h"
#include "../public/GridBase.h"
#include "../public/MissionBase.h"
#include "../../Item/public/ItemBase.h"
#include "../public/TileBase.h"

// Sets default values
AChapterManagerBase::AChapterManagerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChapterManagerBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AChapterManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChapterManagerBase::InitChapterManager()
{
	CurrentChapter = nullptr;
	ChapterLV = 0;
	StatWeight = 0.0f;
	Missions.Empty();

	CreateChapter();
}

void AChapterManagerBase::RemoveMissionItem(AItemBase* target)
{
	GetStageManager()->GetCurrentStage()->Mission->RemoveItem(target);
}


void AChapterManagerBase::RemoveMission(UMissionBase* target)
{
	Missions.Remove(target);
}

void AChapterManagerBase::CreateChapter()
{
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;

	CurrentChapter = GetWorld()->SpawnActor<AGridBase>(AGridBase::StaticClass(), spawnLocation, rotator, spawnParams);
	CurrentChapter->CreateMaze();

	ChapterLV++;
	StatWeight += 0.1f;

	StageManager = GetWorld()->SpawnActor<AStageManagerBase>(AStageManagerBase::StaticClass(), spawnLocation, rotator, spawnParams);
	StageManager->InitStageManager(CurrentChapter);

	CreateMission();
}

void AChapterManagerBase::CreateMission()
{
	TArray<int32> MissionTileidxList;
	TArray<ATileBase*> stageRef = StageManager->GetStages();

	for (int32 i = 0; i < stageRef.Num(); i++)
	{
		MissionTileidxList.Add(i);
	}

	for (int32 i = 0; i < 100; i++)
	{
		int32 SelectidxA = FMath::RandRange(0, stageRef.Num() - 1);
		int32 SelectidxB = FMath::RandRange(0, stageRef.Num() - 1);
		int32 Temp;

		Temp = MissionTileidxList[SelectidxA];
		MissionTileidxList[SelectidxA] = MissionTileidxList[SelectidxB];
		MissionTileidxList[SelectidxB] = Temp;
	}


	for (int32 i = 0; i < MaxMission; i++)
	{
		UMissionBase* target;
		if (i == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[Grid::CreateMaze()] BossMissionTildidx : %d"), MissionTileidxList[i]);
			stageRef[MissionTileidxList[0]]->SetStageType(EStageCategoryInfo::E_Boss);
			target = NewObject<UMissionBase>(this);
			Missions.AddUnique(target);
			target->InitMission(stageRef[MissionTileidxList[0]], 3);
			stageRef[MissionTileidxList[0]]->SetMission(target);
			
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[Grid::CreateMaze()] MissionTildidx : %d"), MissionTileidxList[i]);
			stageRef[MissionTileidxList[i]]->SetStageType(EStageCategoryInfo::E_Mission);
			Missions.AddUnique(NewObject<UMissionBase>(this));
			target = NewObject<UMissionBase>(this);
			Missions.AddUnique(target);
			int8 type = FMath::RandRange(1,2);
			target->InitMission(stageRef[MissionTileidxList[i]], type);
			stageRef[MissionTileidxList[i]]->SetMission(target);
		}
	}
}

void AChapterManagerBase::CleanChapterManager()
{
	StageManager->CleanManager();
	CurrentChapter->RemoveChapter();
}