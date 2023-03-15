// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ChapterManagerBase.h"
#include "../public/StageManagerBase.h"
#include "../public/GridBase.h"
#include "../public/MissionBase.h"
#include "../../Item/public/ItemBase.h"
#include "../public/ALevelScriptInGame.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Engine/LevelStreaming.h"
#include "../public/TileBase.h"
#include "../public/GateBase.h"

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
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;

	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	StageManager = GetWorld()->SpawnActor<AStageManagerBase>(AStageManagerBase::StaticClass(), spawnLocation, rotator, spawnParams);
	StageManager->InitStageManager();
	StageManager->ChapterClearDelegate.AddDynamic(this, &AChapterManagerBase::ClearChapter);
	
	CurrentChapter = nullptr;
	ChapterLV = 0;
	StatWeight = 0.0f;
	Missions.Empty();

	CreateChapter();
}

void AChapterManagerBase::ClearChapter()
{
	if (IsGameClear())
	{
		// Call 게임 클리어 및 종료 
		// 게임 클리어 UI 띄우는 델리게이트 Call
		UE_LOG(LogTemp, Log, TEXT("Game Clear"));
		CleanChapterManager();
		GameModeRef->FinishChapterDelegate.Broadcast(false);
	}
	else
	{
		// 현재 챕터 청소, 보상, 새로운 챕터
		UE_LOG(LogTemp, Log, TEXT("Clear ChapterCall"));
		// 챕터 청소 전 Unload 시키기 현재 move타일에서 언로드 시키고 있음
		CleanChapterManager();
		// 보상
		CreateChapter();
		// UI Update
		InGameScriptRef->SetMiniMapUI();
		StageManager->MoveStage((uint8)EDirection::E_Start);
	}
}


bool AChapterManagerBase::TryAddMissionItem(AItemBase* target)
{
	if (!IsValid(GetStageManager())) return false;

	if (IsValid(GetStageManager()->GetCurrentStage()))
	{
		return GetStageManager()->GetCurrentStage()->Mission->TryAddMissionItem(target);
	}
	return false;
}

bool AChapterManagerBase::TryRemoveMissionItem(AItemBase* target)
{
	if (!IsValid(GetStageManager())) return false;

	if (IsValid(GetStageManager()->GetCurrentStage()))
	{
		return GetStageManager()->GetCurrentStage()->Mission->TryRemoveMissionItem(target);
	}
	return false;
}


void AChapterManagerBase::RemoveMission(UMissionBase* target)
{
	UE_LOG(LogTemp, Log, TEXT("Call RemoveMission !"));
	Missions.Remove(target);

	if (IsChapterClear())
	{
		ULevelStreaming* levelRef = StageManager->GetCurrentStage()->LevelRef;
			ULevel* level = levelRef->GetLoadedLevel();
			for (AActor* actor : level->Actors)
			{
				if (actor != nullptr)
				{
					if (actor->ActorHasTag(FName(TEXT("ChapterGate"))))
					{
						UE_LOG(LogTemp, Log, TEXT("RemoveMission : Find Gate!"));
						Cast<AGateBase>(actor)->ActiveGate();
					}
				}

			}

			// Chapter Timer Pause
			CurrentChapter->PauseTimer();
			// Chapter Clear UI Call 
			InGameScriptRef->PopUpClearUI();
	}
	
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

	StageManager->SetStage(CurrentChapter);
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


	for (int32 i = 0; i < MAX_MISSION_COUNT; i++)
	{
		UMissionBase* target;
		if (i == 0) //보스 미션 초기화
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
			target = NewObject<UMissionBase>(this);
			Missions.AddUnique(target);
			int8 type = FMath::RandRange(1,2);
			target->InitMission(stageRef[MissionTileidxList[i]], 2);
			stageRef[MissionTileidxList[i]]->SetMission(target);
		}
	}
}

void AChapterManagerBase::CleanChapterManager()
{
	UE_LOG(LogTemp, Log, TEXT("Remove Chapter"));

	StageManager->CleanManager();
	CurrentChapter->RemoveChapter();
}