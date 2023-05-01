// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ALevelScriptInGame.h"
#include "../public/ChapterManagerBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Global/public/AssetManagerBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "../public/MissionBase.h"
#include "Engine/LevelStreaming.h"

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
	FadeOutDelegate.AddDynamic(this, &ALevelScriptInGame::FadeOut);


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
	//PerformanceTest();
}

void ALevelScriptInGame::SetGameModeRef()
{
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ALevelScriptInGame::CreateAssetManager()
{
	AssetManager = GetWorld()->SpawnActor<AAssetManagerBase>(AssetManagerBP, FVector::ZeroVector, FRotator::ZeroRotator);
}

void ALevelScriptInGame::PerformanceTest()
{
	TArray<FName> type = { FName("TypeA"),FName("TypeB"),FName("TypeC"),FName("TypeD"),FName("TypeE"),FName("TypeF") };
	ULevelStreaming* ref[9];

	for (int i = 0; i < 9; i++)
	{
		FString name = FString(TEXT("Test"));
		name += FString::FromInt(i);
		int32 mapIdx = FMath::RandRange(0, type.Num() - 1);
		ref[i]= UGameplayStatics::GetStreamingLevel(GetWorld(),type[mapIdx])->CreateInstance(name);
		ref[i]->SetShouldBeLoaded(true);
		ref[i]->SetShouldBeVisible(true);

	}
	
	
	
}