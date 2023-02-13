// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/StageManagerBase.h"
#include "../public/GridBase.h"
#include "../public/TileBase.h"
#include "../public/ChapterManagerBase.h"
#include "Engine/LevelStreaming.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "../public/ALevelScriptInGame.h"
#include "../public/LevelScriptBase.h"


// Sets default values
AStageManagerBase::AStageManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageManagerBase::InitStageManager(AGridBase* Chapter)
{
	CharacterRef = Cast<AMainCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//GamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));


	for (ATileBase* tile : Chapter->StageArray)
	{
		Stages.Add(tile);
	}

	CurrentTile = nullptr;
	SetMissionStages();
}

void AStageManagerBase::CleanManager()
{
	Stages.Empty();
	CurrentTile = nullptr;
}

void AStageManagerBase::SetMissionStages()
{
	
}

void AStageManagerBase::MoveStage(uint8 Dir)
{
	ATileBase* toUnLoadStage = nullptr;

	if (CurrentTile != nullptr)
		toUnLoadStage = CurrentTile;

	switch ((EDirection)Dir)
	{
	case EDirection::E_UP:
		CurrentTile = GetStage(CurrentTile->XPos, CurrentTile->YPos + 1);
		LoadStage(CurrentTile);
		UE_LOG(LogTemp, Log, TEXT("Move to Up"));
		break;
	case EDirection::E_DOWN:
		CurrentTile = GetStage(CurrentTile->XPos, CurrentTile->YPos - 1);
		LoadStage(CurrentTile);
		UE_LOG(LogTemp, Log, TEXT("Move to Down"));
		break;
	case EDirection::E_LEFT:
		CurrentTile = GetStage(CurrentTile->XPos - 1, CurrentTile->YPos);
		LoadStage(CurrentTile);
		UE_LOG(LogTemp, Log, TEXT("Move to Left"));
		break;
	case EDirection::E_RIGHT:
		CurrentTile = GetStage(CurrentTile->XPos + 1, CurrentTile->YPos);
		LoadStage(CurrentTile);
		UE_LOG(LogTemp, Log, TEXT("Move to Right"));
		break;
	case EDirection::E_Start:
		UE_LOG(LogTemp, Log, TEXT("Start Game"));
		CurrentTile = Stages[0];
		LoadStage(CurrentTile);
		break;
	case EDirection::E_Chapter:
		UE_LOG(LogTemp, Log, TEXT("New Chapter"));
		LoadStage(CurrentTile);
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("Wrong Dir"));
		break;
	}
	
	if (toUnLoadStage != nullptr)
		UnLoadStage(toUnLoadStage);
	
	// UI 업데이트
	InGameScriptRef->UpdateMiniMapUI();
}

void AStageManagerBase::LoadStage(ATileBase* targetStage)
{
	if (targetStage->LevelRef != nullptr) 
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));
		targetStage->LevelRef->SetShouldBeLoaded(true);
		targetStage->LevelRef->SetShouldBeVisible(true);

		CharacterRef->SetActorLocation(targetStage->GetActorLocation() + FVector(0, 0, 1500));
		// Timer
		//GetWorldTimerManager().UnPauseTimer(ClearTimerHandle);

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , Create Level Instance"));
		FString name = FString::FromInt(InGameScriptRef->ChapterManager->GetChapterLV());
		name += FString(TEXT("Stage"));
		name += FString::FromInt(targetStage->YPos * 5 + targetStage->XPos);
		targetStage->LevelRef = UGameplayStatics::GetStreamingLevel(GetWorld(), targetStage->LevelToLoad)->CreateInstance(name);
		targetStage->LevelRef->LevelTransform.SetLocation(targetStage->GetActorLocation());

		targetStage->LevelRef->SetShouldBeLoaded(true);
		targetStage->LevelRef->SetShouldBeVisible(true);
	
		CharacterRef->SetActorLocation(targetStage->GetActorLocation() + FVector(0, 0, 1500));
		// Clear Timer 설정
		//GetWorldTimerManager().SetTimer(ClearTimerHandle, this, &ATileBase::SetReward, 60.0f, true);

	}
}

void AStageManagerBase::UnLoadStage(ATileBase* targetStage)
{
	if (targetStage->LevelRef != nullptr) // 레벨 스트리밍 인스턴스 존재
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Now UnLoad Level"));
		targetStage->LevelRef->SetShouldBeLoaded(false);
		targetStage->LevelRef->SetShouldBeVisible(false);
	
		for (AEnemyCharacterBase* Target : targetStage->MonsterList)
		{
			UE_LOG(LogTemp, Log, TEXT("Call SetMovementMode"));
			Target->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
		// Pause Timer
		//GetWorldTimerManager().PauseTimer(ClearTimerHandle);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , error"));

	}
}

ATileBase* AStageManagerBase::GetStage(int32 XPosition, int32 YPosition)
{
	int32 size = GridSize;

	if (XPosition >= 0 && XPosition < size && YPosition >= 0 && YPosition < size)
	{
		return (Stages[(YPosition * size) + XPosition]);
	}

	return nullptr;
}
