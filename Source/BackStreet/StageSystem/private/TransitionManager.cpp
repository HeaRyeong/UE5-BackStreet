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
		chapterManager->SetCurrentStage(GetStage(currentStage->GetXPos() + 1, currentStage->GetYPos()));
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Up"));
		break;
	case EDirection::E_DOWN:
		chapterManager->SetCurrentStage(GetStage(currentStage->GetXPos() - 1, currentStage->GetYPos()));
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Down"));
		break;
	case EDirection::E_LEFT:
		chapterManager->SetCurrentStage(GetStage(currentStage->GetXPos(), currentStage->GetYPos() - 1));
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Left"));
		break;
	case EDirection::E_RIGHT:
		chapterManager->SetCurrentStage(GetStage(currentStage->GetXPos(), currentStage->GetYPos() + 1));
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

	if (currentStage->GetLevelRef() != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));

		currentStage->GetLevelRef()->OnLevelShown.Add(LoadCompleteDelegate);

		currentStage->GetLevelRef()->SetShouldBeLoaded(true);
		currentStage->GetLevelRef()->SetShouldBeVisible(true);


	}
	else
	{
		AChapterManagerBase* chapterManager = Cast<AChapterManagerBase>(this->GetOuter());
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , Create Level Instance"));

		FString name = FString::FromInt(chapterManager->GetChapterLV());
		FVector location = currentStage->GetStageLocation();

		name += FString(TEXT("Stage"));
		name += FString::FromInt(currentStage->GetYPos() * MAX_GRID_SIZE + currentStage->GetXPos());

		currentStage->SetLevelRef(UGameplayStatics::GetStreamingLevel(GetWorld(), currentStage->GetLevelToLoad())->CreateInstance(name));
		ULevelStreaming* level = currentStage->GetLevelRef();

		currentStage->GetLevelRef()->LevelTransform.SetLocation(location);
		
		LoadCompleteDelegate.BindUFunction(this, "CompleteLoad");
		level->OnLevelShown.Add(LoadCompleteDelegate);
		currentStage->GetLevelRef()->SetShouldBeLoaded(true);
		currentStage->GetLevelRef()->SetShouldBeVisible(true);

	}

}

void UTransitionManager::UnLoadStage()
{
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::UnLoadStage"));
	//FScriptDelegate UnloadCompleteDelegate;

	if (HideStage != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Now UnLoad Level"));
		HideStage->GetLevelRef()->OnLevelHidden.Add(UnloadCompleteDelegate);
		HideStage->GetLevelRef()->SetShouldBeLoaded(false);
		HideStage->GetLevelRef()->SetShouldBeVisible(false);
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
	if (!currentStage->GetIsVisited())
	{
		SetStage(currentStage);
		currentStage->SetIsVisited(true);
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

	ULevelStreaming* levelRef = Target->GetLevelRef();
	ULevel* level = levelRef->GetLoadedLevel();

	for (AActor* actor : level->Actors)
	{
		if (actor->ActorHasTag(FName(TEXT("MonsterSpawnPoint"))))
		{
			Target->AddMonsterSpawnPoint(FVector(actor->GetActorLocation()));
		}
		if (actor->ActorHasTag(FName(TEXT("ItemSpawnPoint"))))
		{
			Target->AddItemSpawnPoint(FVector(actor->GetActorLocation()));
		}
		if (actor->ActorHasTag(FName(TEXT("CharacterSpawnPoint"))))
		{
			Target->AddCharacterSpawnPoint(FVector(actor->GetActorLocation()));
		}
		if (actor->ActorHasTag(FName(TEXT("RewardBoxSpawnPoint"))))
		{
			Target->AddRewardBoxSpawnPoint(FVector(actor->GetActorLocation()));
		}
	}
}

void UTransitionManager::SetGate()
{
	UE_LOG(LogTemp, Log, TEXT("UTransitionManager::SetGate"));

	TArray<AActor*> gates;
	AStageData* currentStage = Cast<AChapterManagerBase>(this->GetOuter())->GetCurrentStage();
	currentStage->EmptyGateList();

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
		TArray<FVector> characterSpawnPoint = currentStage->GetCharacterSpawnPoint();

		if (characterSpawnPoint.IsValidIndex(0))
		{
			moveLocation = characterSpawnPoint[0];

		}
	}
	else
	{
		for (class AGateBase* gate : currentStage->GetGateList())
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
	if (XPosition >= 0 && XPosition < MAX_GRID_SIZE && YPosition >= 0 && YPosition < MAX_GRID_SIZE)
	{
		return (Stages[(YPosition * MAX_GRID_SIZE) + XPosition]);
	}
	else
		return nullptr;
}