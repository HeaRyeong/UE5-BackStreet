// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/TransitionManager.h"
#include "../public/ChapterManagerBase.h"
#include "../public/ResourceManager.h"
#include "../public/StageData.h"
#include "../public/GateBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "Engine/LevelStreaming.h"

void UTransitionManager::InitTransitionManager()
{
	// Delegate 왜 안되지..????
	/*if (!SpawnRequestDelegate.IsBound())
	{
		SpawnRequestDelegate.AddDynamic(this, &AResourceManager::SpawnStageActor);
	}*/
	LoadCompleteDelegate.BindUFunction(this, "CompleteLoad");
	UnloadCompleteDelegate.BindUFunction(this, "SetGate");

}

void UTransitionManager::InitChapter(TArray<class AStageData*> StageRef)
{
	Stages = StageRef;
	HideStage = nullptr;
}

void UTransitionManager::MoveStage(EDirection Dir)
{
	AStageData* currentStage = Cast<AChapterManagerBase>(this->GetOuter())->GetCurrentStage();
	AChapterManagerBase* chapterManager = Cast<AChapterManagerBase>(this->GetOuter());

	MoveDirection = Dir;
	HideStageWork(Dir);

	switch (Dir)
	{
	case EDirection::E_UP:
		chapterManager->SetCurrentStage(GetStage(currentStage->XPos + 1, currentStage->YPos));
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Up"));
		break;
	case EDirection::E_DOWN:
		chapterManager->SetCurrentStage(GetStage(currentStage->XPos - 1, currentStage->YPos));
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Down"));
		break;
	case EDirection::E_LEFT:
		chapterManager->SetCurrentStage(GetStage(currentStage->XPos, currentStage->YPos - 1));
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Left"));
		break;
	case EDirection::E_RIGHT:
		chapterManager->SetCurrentStage(GetStage(currentStage->XPos, currentStage->YPos + 1));
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Right"));
		break;
	case EDirection::E_Start:
		UE_LOG(LogTemp, Log, TEXT("Start Game"));
		chapterManager->SetCurrentStage(Stages[0]);
		LoadStage();
		break;
	case EDirection::E_Chapter:
		UE_LOG(LogTemp, Log, TEXT("New Chapter"));
		chapterManager->SetCurrentStage(chapterManager->GetLobbyStage());
		UnLoadStage();
		chapterManager->MoveChapter();
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("Wrong Dir"));
		break;
	}

	Cast<AChapterManagerBase>(this->GetOuter())->UpdateMapUI();

}

void UTransitionManager::LoadStage()
{
	AStageData* currentStage = Cast<AChapterManagerBase>(this->GetOuter())->GetCurrentStage();
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::LoadStage"));
	//FScriptDelegate loadCompleteDelegate;

	if (currentStage->LevelRef != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));

		currentStage->LevelRef->OnLevelShown.Add(LoadCompleteDelegate);

		currentStage->LevelRef->SetShouldBeLoaded(true);
		currentStage->LevelRef->SetShouldBeVisible(true);


	}
	else
	{
		AChapterManagerBase* chapterManager = Cast<AChapterManagerBase>(this->GetOuter());
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , Create Level Instance"));

		FString name = FString::FromInt(chapterManager->GetChapterLV());
		FVector location = currentStage->StageLocation;

		name += FString(TEXT("Stage"));
		name += FString::FromInt(currentStage->YPos * MAX_GRID_SIZE + currentStage->XPos);

		currentStage->LevelRef = UGameplayStatics::GetStreamingLevel(GetWorld(), currentStage->LevelToLoad)->CreateInstance(name);
		ULevelStreaming* level = currentStage->LevelRef;

		currentStage->LevelRef->LevelTransform.SetLocation(location);
		
		LoadCompleteDelegate.BindUFunction(this, "CompleteLoad");
		level->OnLevelShown.Add(LoadCompleteDelegate);
		currentStage->LevelRef->SetShouldBeLoaded(true);
		currentStage->LevelRef->SetShouldBeVisible(true);

	}

}

void UTransitionManager::UnLoadStage()
{
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::UnLoadStage"));
	//FScriptDelegate UnloadCompleteDelegate;

	if (HideStage != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Now UnLoad Level"));
		HideStage->LevelRef->OnLevelHidden.Add(UnloadCompleteDelegate);
		HideStage->LevelRef->SetShouldBeLoaded(false);
		HideStage->LevelRef->SetShouldBeVisible(false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist"));
		SetGate();
	}

}

void UTransitionManager::CompleteLoad()
{
	UE_LOG(LogTemp, Log, TEXT("UTransitionManager::CompleteLoad"));
	AStageData* currentStage = Cast<AChapterManagerBase>(this->GetOuter())->GetCurrentStage();
	if (!currentStage->bIsVisited)
	{
		SetStage(currentStage);
		currentStage->bIsVisited = true;
		// 델리게이트로 변경필요
		Cast<AChapterManagerBase>(this->GetOuter())->GetResourceManager()->SpawnStageActor(currentStage);
	}
	else
	{
		currentStage->AIOnDelegate.Broadcast();
	}

	UnLoadStage();

}

void UTransitionManager::SetStage(AStageData* Target)
{
	UE_LOG(LogTemp, Log, TEXT("UTransitionManager::SetStage"));
	SetSpawnPoint(Target);
	SpawnRequestDelegate.Broadcast(Target);
}

void UTransitionManager::SetSpawnPoint(AStageData* Target)
{
	UE_LOG(LogTemp, Log, TEXT("UTransitionManager::SetSpawnPoint"));

	ULevelStreaming* levelRef = Target->LevelRef;
	ULevel* level = levelRef->GetLoadedLevel();

	for (AActor* actor : level->Actors)
	{
		if (actor->ActorHasTag(FName(TEXT("MonsterSpawnPoint"))))
		{
			Target->MonsterSpawnPoints.AddUnique(FVector(actor->GetActorLocation()));

		}
		if (actor->ActorHasTag(FName(TEXT("ItemSpawnPoint"))))
		{
			Target->ItemSpawnPoints.AddUnique(FVector(actor->GetActorLocation()));
		}
		if (actor->ActorHasTag(FName(TEXT("CharacterSpawnPoint"))))
		{
			Target->CharacterSpawnPoint.AddUnique(FVector(actor->GetActorLocation()));

		}
		if (actor->ActorHasTag(FName(TEXT("RewardBoxSpawnPoint"))))
		{
			Target->RewardBoxSpawnPoint.AddUnique(FVector(actor->GetActorLocation()));

		}
	}
}

void UTransitionManager::SetGate()
{
	UE_LOG(LogTemp, Log, TEXT("UTransitionManager::SetGate"));

	TArray<AActor*> gates;
	AStageData* currentStage = Cast<AChapterManagerBase>(this->GetOuter())->GetCurrentStage();
	currentStage->GateList.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateBase::StaticClass(), gates);
	for (AActor* gate : gates)
	{
		AGateBase* target = Cast<AGateBase>(gate);
		target->InitGate();

	}
	TeleportCharacter();

}

void UTransitionManager::HideStageWork(EDirection Dir)
{	
	if (Dir != EDirection::E_Start)
	{
		HideStage = Cast<AChapterManagerBase>(this->GetOuter())->GetCurrentStage();
		HideStage->AIOffDelegate.Broadcast();

	}
	else
		HideStage = nullptr;
}

void UTransitionManager::TeleportCharacter()
{
	UE_LOG(LogTemp, Log, TEXT("UTransitionManager::TeleportCharacter"));
	AStageData* currentStage = Cast<AChapterManagerBase>(this->GetOuter())->GetCurrentStage();
	AMainCharacterBase* characterRef = Cast<AMainCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	AChapterManagerBase* chapterManager = Cast<AChapterManagerBase>(this->GetOuter());
	FVector moveLocation;
	FName telegate;

	switch (MoveDirection)
	{
	case EDirection::E_UP:
		telegate = FName(TEXT("DOWN"));
		break;
	case EDirection::E_DOWN:
		telegate = FName(TEXT("UP"));
		break;
	case EDirection::E_LEFT:
		telegate = FName(TEXT("RIGHT"));
		break;
	case EDirection::E_RIGHT:
		telegate = FName(TEXT("LEFT"));
		break;
	case EDirection::E_Start:
		telegate = FName(TEXT("StartGate"));
		break;
	case EDirection::E_Chapter:
		telegate = FName(TEXT("ChapterGate"));
		break;
	}

	if (telegate == FName(TEXT("ChapterGate")))
	{
		 TArray<AActor*> gates;

		 UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateBase::StaticClass(), gates);
		 for (AActor* target : gates)
		 {
			 if (target->ActorHasTag(FName("StartGate")))
				 moveLocation = target->GetActorLocation();
		 }
	}
	else if (telegate == FName(TEXT("StartGate")))
	{
		if (currentStage->CharacterSpawnPoint.IsValidIndex(0))
		{
			moveLocation = currentStage->CharacterSpawnPoint[0];

		}
	}
	else
	{
		for (AGateBase* gate : currentStage->GateList)
		{
			if (gate->ActorHasTag(telegate))
			{
				UE_LOG(LogTemp, Log, TEXT("GateCheck!"));
				moveLocation = gate->GetActorLocation();
				break;
			}

		}

	}
	moveLocation = moveLocation + FVector(0, 0, 10);
	if(IsValid(characterRef)) characterRef->SetActorLocation(moveLocation);
	else { UE_LOG(LogTemp, Warning, TEXT("Whyrano")); }
}

AStageData* UTransitionManager::GetStage(uint8 XPosition, uint8 YPosition)
{
	if (XPosition >= 0 && XPosition < ChapterSize && YPosition >= 0 && YPosition < ChapterSize)
	{
		return (Stages[(YPosition * ChapterSize) + XPosition]);
	}
	else
		return nullptr;
}