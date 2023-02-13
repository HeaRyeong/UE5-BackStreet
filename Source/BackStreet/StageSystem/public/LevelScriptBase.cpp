// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/LevelScriptBase.h"
#include "../public/GateBase.h"
#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Global/public/AssetManagerBase.h"
#include "../public/ChapterManagerBase.h"
#include "../public/StageManagerBase.h"
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
	
	if (InGameScriptRef != nullptr && InGameScriptRef->ChapterManager != nullptr)
	{
		ATileBase* belongTile = InGameScriptRef->ChapterManager->GetStageManager()->GetCurrentStage();
			if (belongTile != nullptr)
			{
				if(!belongTile->bIsVisited)
						InitLevel(belongTile);
			}
	}
	
}

void ALevelScriptBase::InitLevel(ATileBase* target)
{
	UE_LOG(LogTemp, Log, TEXT("InitLevel!"));
	SetGate();
	SetSpawnPoints(target);
	target->SetStage();
	target->bIsVisited = true;
}

void ALevelScriptBase::SetGate()
{
	TArray<AActor*> gates;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateBase::StaticClass(), gates);
	for (AActor* gate : gates)
	{
		UE_LOG(LogTemp, Log, TEXT("Find Gate and Init Gate"));
		AGateBase* target = Cast<AGateBase>(gate);
		target->InitGate();

	}
}

void ALevelScriptBase::SetSpawnPoints(ATileBase* target)
{
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("MonsterSpawnPoint")), target->MonsterSpawnPoints);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("ItemSpawnPoint")), target->ItemSpawnPoints);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("MissionSpawnPoint")), target->MissionSpawnPoints);
}
