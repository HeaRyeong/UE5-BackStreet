// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ChapterManagerBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../public/TransitionManager.h"
#include "../public/StageGenerator.h"
#include "../public/ResourceManager.h"
#include "../public/StageData.h"
#include "../public/GateBase.h"

AChapterManagerBase::AChapterManagerBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AChapterManagerBase::BeginPlay()
{
	Super::BeginPlay();

}

void AChapterManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChapterManagerBase::SetLobbyStage()
{
	TArray<AActor*> lobbyStages;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStageData::StaticClass(), lobbyStages);
	for (AActor* target : lobbyStages)
	{
		if (Cast<AStageData>(target)->GetStageType() == EStageCategoryInfo::E_Lobby)
		{
			UE_LOG(LogTemp, Log, TEXT("AChapterManagerBase::SetLobbyStage: Find LobbyStage"));

			LobbyStage = Cast<AStageData>(target);

		}
	}
}

bool AChapterManagerBase::CheckChapterClear()
{
	if (IsChapterClear())
	{
		// 챕터 클리어 관련 로직 실행
		UE_LOG(LogTemp, Log, TEXT("AChapterManagerBase::CheckChapterClear: Clear Chapter"));

		TArray<AActor*> gates;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateBase::StaticClass(), gates);
		for (AActor* gate : gates)
		{
			AGateBase* target = Cast<AGateBase>(gate);
			if(target->ActorHasTag(FName("ChapterGate")))
				target->ActivateChapterGate();
		}
		return true;
	}
	else return false;
}

bool AChapterManagerBase::IsChapterClear()
{
	for (AStageData* stage : StageList)
	{
		if (stage->GetStageType() == EStageCategoryInfo::E_Boss)
		{
			if (stage->bIsClear)
				return true;
		}
	}
	return false;
}
void AChapterManagerBase::MoveChapter()
{
	UE_LOG(LogTemp, Log, TEXT("AChapterManagerBase: MoveChapter"));
	ABackStreetGameModeBase* gameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (IsGameClear())
	{
		UE_LOG(LogTemp, Log, TEXT("MoveChapte:Game Clear"));
		ResourceManager->CleanAllResource();
		gameModeRef->FinishChapterDelegate.Broadcast(false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("MoveChapter:Move to Next Chapter"));

		ResourceManager->CleanAllResource();
		CreateChapter();
		InitChapterManager();
		// UI Update
		gameModeRef->SetMiniMapUI();
		gameModeRef->UpdateMiniMapUI();
	}


}

void AChapterManagerBase::CreateChapterManager()
{
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;

	ChapterLV = 0;
	StatWeight = 0.0f;

	StageGenerator = NewObject<UStageGenerator>(this);
	TransitionManager = NewObject<UTransitionManager>(this);


	CreateResourceManager();
	CreateChapter();
	SetLobbyStage();
	InitChapterManager();
}

void AChapterManagerBase::CreateResourceManager()
{
	UObject* spawnResourceManager = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/System/StageManager/Blueprint/BP_ResourceManager.BP_ResourceManager")));

	UBlueprint* bp = Cast<UBlueprint>(spawnResourceManager);
	if (!spawnResourceManager)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return;
	}

	UClass* spawnClass = spawnResourceManager->StaticClass();
	if (spawnClass == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return;
	}

	UWorld* world = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ResourceManager = Cast<AResourceManager>(world->SpawnActor<AActor>(bp->GeneratedClass, GetActorLocation(), GetActorRotation(), SpawnParams));

}

void AChapterManagerBase::InitChapterManager()
{
	// Level에 있는 초기화 필요한 Actor 초기화시키기 , 바인딩 , 및 참조 초기화 코드 등
	TransitionManager->InitTransitionManager();
	TransitionManager->InitChapter(StageList);
	CurrentStage = LobbyStage;
}

void AChapterManagerBase::CreateChapter()
{
	StageList=StageGenerator->CreateMaze();
	CurrentStage = nullptr;
	ChapterLV++;
	StatWeight += 0.1f;
}

void AChapterManagerBase::InitStartGate()
{
	TArray<AActor*> gates;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateBase::StaticClass(), gates);
	for (AActor* gate : gates)
	{
		AGateBase* target = Cast<AGateBase>(gate);
		target->InitGate();
	}
}

void AChapterManagerBase::UpdateMapUI()
{

	Cast<ABackStreetGameModeBase>(GetOwner())->UpdateMiniMapUI();
}

