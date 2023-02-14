// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "ALevelScriptInGame.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API ALevelScriptInGame : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	ALevelScriptInGame();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable)
		void StartGame();

	UFUNCTION(BlueprintCallable)
		void SetGameModeRef();


public:
	UPROPERTY()
		class AChapterManagerBase* ChapterManager;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class AChapterManagerBase* GetChapterManager() { return ChapterManager; }

public:
	UFUNCTION()
		void CreateAssetManager();

	UFUNCTION(BlueprintCallable)
		class AAssetManagerBase* GetAssetManager() { return AssetManager; }

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateMiniMapUI();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AAssetManagerBase> AssetManagerBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAssetManagerBase* AssetManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABackStreetGameModeBase* GameModeRef;

	
};
