// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/LevelScriptBase.h"
#include "../public/GateBase.h"
#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Global/public/AssetManagerBase.h"
#include "../public/ChapterManagerBase.h"
#include "../public/StageManagerBase.h"
#include "Engine/LevelStreaming.h"
#include "../public/ALevelScriptInGame.h"

ALevelScriptBase::ALevelScriptBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called every frame
void ALevelScriptBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelScriptBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Call ALevelScriptBase BeginPlay!"));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	GameModeRef->ClearResourceDelegate.AddDynamic(this, &ALevelScriptBase::ClearAllTimerHandle);
	InitLevelSequence();
	
	PlayLoadSequencePlayer();
	

	GetWorldTimerManager().SetTimer(ResourceReturnTimerHandle, FTimerDelegate::CreateLambda([&]() {
		UE_LOG(LogTemp, Log, TEXT("Call Timer"));
		InGameScriptRef->ChapterManager->GetStageManager()->UnLoadStage();

		if (InGameScriptRef != nullptr && InGameScriptRef->ChapterManager != nullptr)
		{
			BelongTileRef = InGameScriptRef->ChapterManager->GetStageManager()->GetCurrentStage();
			if (BelongTileRef != nullptr)
			{
				BelongTileRef->ScriptRef = this;
				if (!(BelongTileRef->bIsVisited))
					InitLevel(BelongTileRef);
			}
		}
		SetGate();
		TeleportCharacter();
		BelongTileRef->UnPauseStage();
		ClearAllTimerHandle();
		}), 1.0f, false, 0.75f);



	/*if (InGameScriptRef != nullptr && InGameScriptRef->ChapterManager != nullptr)
	{
		BelongTileRef = InGameScriptRef->ChapterManager->GetStageManager()->GetCurrentStage();
		if (BelongTileRef != nullptr)
		{
			BelongTileRef->ScriptRef = this;
			if (!(BelongTileRef->bIsVisited))
				InitLevel(BelongTileRef);
		}
	}
	SetGate();
	TeleportCharacter();*/
}

void ALevelScriptBase::InitLevel(ATileBase* target)
{
	UE_LOG(LogTemp, Log, TEXT("InitLevel!"));
	BelongTileRef = target;
	SetSpawnPoints(target);
	target->SetStage();
	target->bIsVisited = true;
}

void ALevelScriptBase::TeleportCharacter()
{
	TArray<AActor*> gates;
	FName telegate;

	switch (InGameScriptRef->ChapterManager->GetStageManager()->GetMoveDir())
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
		telegate = FName(TEXT("ChapterGate"));
		break;
	case EDirection::E_Chapter:
		telegate = FName(TEXT("ChapterGate"));
		break;
	}

	
	ULevelStreaming* levelRef = BelongTileRef->LevelRef;
	ULevel* level = levelRef->GetLoadedLevel();
	for (AActor* actor : level->Actors)
	{
		if (actor != nullptr)
		{
			if (actor->ActorHasTag(FName(TEXT("Gate"))))
			{
				gates.Add(Cast<AGateBase>(actor));
			}
		}

	}
	if (gates.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("gate is empty!"));
	}

	if (telegate == FName(TEXT("ChapterGate")))
	{
		if (BelongTileRef->CharacterSpawnPoint[0] != nullptr)
		{
			ACharacterBase* CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			CharacterRef->SetActorLocation(BelongTileRef->CharacterSpawnPoint[0]->GetActorLocation());
		}
	}
	else
	{
		for (AActor* gate : gates)
		{
			AGateBase* target = Cast<AGateBase>(gate);
			if (gate->ActorHasTag(telegate))
			{
				UE_LOG(LogTemp, Log, TEXT("GateCheck!"));
				ACharacterBase* CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				CharacterRef->SetActorLocation(gate->GetActorLocation());
				break;
			}

		}
	
	}

	
}

void ALevelScriptBase::SetGate()
{
	TArray<AActor*> gates;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateBase::StaticClass(), gates);
	for (AActor* gate : gates)
	{
		AGateBase* target = Cast<AGateBase>(gate);
		target->InitGate();

	}
}

void ALevelScriptBase::SetSpawnPoints(ATileBase* target)
{
	UE_LOG(LogTemp, Log, TEXT("Call SetSpawnPoints!"));
	ULevelStreaming* levelRef = target->LevelRef;
	ULevel* level = levelRef->GetLoadedLevel();
	for (AActor* actor : level->Actors)
	{ 
		if (actor->ActorHasTag(FName(TEXT("MonsterSpawnPoint"))))
		{
			target->MonsterSpawnPoints.AddUnique(actor);
		}
		if (actor->ActorHasTag(FName(TEXT("ItemSpawnPoint"))))
		{
			target->ItemSpawnPoints.AddUnique(actor);
		}
		if (actor->ActorHasTag(FName(TEXT("MissionSpawnPoint"))))
		{
			target->MissionSpawnPoints.AddUnique(actor);
		}
		if (actor->ActorHasTag(FName(TEXT("CharacterSpawnPoint"))))
		{
			target->CharacterSpawnPoint.AddUnique(actor);

		}
	}
}

void ALevelScriptBase::PlayLoadSequencePlayer()
{
	if (IsValid(LoadSequencePlayer))
	{
		LoadSequencePlayer->Play();
	}

}


void ALevelScriptBase::InitLevelSequence()
{
	ULevelSequence* loadEffectSequence = Cast<ULevelSequence>(InGameScriptRef->GetAssetManager()->GetFadeInEffectSequence());

	if (!IsValid(loadEffectSequence)) return;
	ALevelSequenceActor* loadActor = nullptr;

	LoadSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), loadEffectSequence
		, FMovieSceneSequencePlaybackSettings(), loadActor);

}

void ALevelScriptBase::ClearAllTimerHandle()
{
	GetWorldTimerManager().ClearTimer(TravelSequenceDelayHandle);
	GetWorldTimerManager().ClearTimer(ResourceReturnTimerHandle);
}
