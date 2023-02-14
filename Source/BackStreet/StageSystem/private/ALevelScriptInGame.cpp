// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ALevelScriptInGame.h"
#include "../public/ChapterManagerBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Global/public/AssetManagerBase.h"

ALevelScriptInGame::ALevelScriptInGame()
{


}


// Called every frame
void ALevelScriptInGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelScriptInGame::BeginPlay()
{
	Super::BeginPlay();
	
	//SetGameModeRef()
	//StartGame();
}


void ALevelScriptInGame::StartGame()
{
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;

	if (AssetManager == nullptr)
		CreateAssetManager();

	ChapterManager = GetWorld()->SpawnActor<AChapterManagerBase>(AChapterManagerBase::StaticClass(), spawnLocation, rotator, spawnParams);
	ChapterManager->InitChapterManager();
	GameModeRef->StartChapter();
}

void ALevelScriptInGame::SetGameModeRef()
{
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ALevelScriptInGame::CreateAssetManager()
{
	AssetManager = GetWorld()->SpawnActor<AAssetManagerBase>(AssetManagerBP, FVector::ZeroVector, FRotator::ZeroRotator);
}