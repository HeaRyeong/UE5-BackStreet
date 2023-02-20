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
	//InitTileTravelSequence();
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
	if (CurrentTile != nullptr)
	{
		UnloadTile = CurrentTile;
		UnloadTile->DeactivateAI();
	}

	MoveDir = (EDirection)Dir;

	switch ((EDirection)Dir)
	{
	case EDirection::E_UP:
		CurrentTile = GetStage(CurrentTile->XPos + 1, CurrentTile->YPos);
		LoadStage();
		CurrentTile->ActivateAI();
		UE_LOG(LogTemp, Log, TEXT("Move to Up"));
		break;
	case EDirection::E_DOWN:
		CurrentTile = GetStage(CurrentTile->XPos - 1, CurrentTile->YPos);
		LoadStage();
		CurrentTile->ActivateAI();
		UE_LOG(LogTemp, Log, TEXT("Move to Down"));
		break;
	case EDirection::E_LEFT:
		CurrentTile = GetStage(CurrentTile->XPos, CurrentTile->YPos - 1);
		LoadStage();
		CurrentTile->ActivateAI();
		UE_LOG(LogTemp, Log, TEXT("Move to Left"));
		break;
	case EDirection::E_RIGHT:
		CurrentTile = GetStage(CurrentTile->XPos, CurrentTile->YPos + 1);
		LoadStage();
		CurrentTile->ActivateAI();
		UE_LOG(LogTemp, Log, TEXT("Move to Right"));
		break;
	case EDirection::E_Start:
		UE_LOG(LogTemp, Log, TEXT("Start Game"));
		CurrentTile = Stages[0];
		LoadStage();
		CurrentTile->ActivateAI();
		break;
	case EDirection::E_Chapter:
		UE_LOG(LogTemp, Log, TEXT("New Chapter"));
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("Wrong Dir"));
		break;
	}
	

	// UI 업데이트
	InGameScriptRef->UpdateMiniMapUI();
}

void AStageManagerBase::LoadStage()
{
	//FScriptDelegate MyScriptDelegate;

	if (CurrentTile->LevelRef != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));
		CurrentTile->LevelRef->SetShouldBeLoaded(true);
		CurrentTile->LevelRef->SetShouldBeVisible(true);
		//MyScriptDelegate.BindUFunction(this, "CompleteLoad");
		//CurrentTile->LevelRef->OnLevelLoaded.Add(MyScriptDelegate);
		//CharacterRef->SetActorLocation(targetStage->GetActorLocation() + FVector(0, 0, 1500));
		// Timer
		//GetWorldTimerManager().UnPauseTimer(ClearTimerHandle);

	}
	else
	{
		
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , Create Level Instance"));
		FString name = FString::FromInt(InGameScriptRef->ChapterManager->GetChapterLV());
		name += FString(TEXT("Stage"));
		name += FString::FromInt(CurrentTile->YPos * GridSize + CurrentTile->XPos);
		CurrentTile->LevelRef = UGameplayStatics::GetStreamingLevel(GetWorld(), CurrentTile->LevelToLoad)->CreateInstance(name);
		CurrentTile->LevelRef->LevelTransform.SetLocation(CurrentTile->GetActorLocation());
		//MyScriptDelegate.BindUFunction(this, "CompleteLoad");
		//CurrentTile->LevelRef->OnLevelLoaded.Add(MyScriptDelegate);
		CurrentTile->LevelRef->SetShouldBeLoaded(true);
		CurrentTile->LevelRef->SetShouldBeVisible(true);
	
		//CharacterRef->SetActorLocation(targetStage->GetActorLocation() + FVector(0, 0, 1500));
		// Clear Timer 설정
		//GetWorldTimerManager().SetTimer(ClearTimerHandle, this, &ATileBase::SetReward, 60.0f, true);

	}
}

void AStageManagerBase::UnLoadStage()
{
	if (UnloadTile != nullptr)
	{
		if (UnloadTile->LevelRef != nullptr) // 레벨 스트리밍 인스턴스 존재
		{
			UE_LOG(LogTemp, Log, TEXT("Instance is exist, Now UnLoad Level"));
			UnloadTile->LevelRef->SetShouldBeLoaded(false);
			UnloadTile->LevelRef->SetShouldBeVisible(false);
			//UnloadDelegate.BindUFunction(this, "CompleteUnLoad");
			//CurrentTile->LevelRef->OnLevelLoaded.Add(UnloadDelegate);
		
			// Pause Timer
			//GetWorldTimerManager().PauseTimer(ClearTimerHandle);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Instance is not exist , error"));
	
		}
	}
	else
	{
		//CompleteUnLoad();
	}

}

void AStageManagerBase::CompleteLoad()
{
	UE_LOG(LogTemp, Log, TEXT("Delegate Call CompleteLoad"));
}

void AStageManagerBase::CompleteUnLoad()
{/*
	UE_LOG(LogTemp, Log, TEXT("Delegate Call CompleteUnLoad"));
	ALevelScriptBase* script = Cast<ALevelScriptBase>(CurrentTile->LevelRef->GetLoadedLevel()->GetLevelScriptActor());
	
	script->InitLevel(CurrentTile);
	script->SetGate();
	script->TeleportCharacter();*/
}

ATileBase* AStageManagerBase::GetStage(int32 XPosition, int32 YPosition)
{
	int32 size = GridSize;

	if (XPosition >= 0 && XPosition < size && YPosition >= 0 && YPosition < size)
	{
		return (Stages[(YPosition * size) + XPosition]);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Out of Range X:%d Y:%d"), XPosition,YPosition);
		return nullptr;
	}
}
