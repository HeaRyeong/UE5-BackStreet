// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ChapterManagerBase.generated.h"
// Chapter °¹¼ö

#define MAX_CHAPTER_COUNT 2



UCLASS()
class BACKSTREET_API AChapterManagerBase : public AActor
{
	GENERATED_BODY()


public:
	AChapterManagerBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void SetLobbyStage();

	UFUNCTION()
		void CreateChapterManager();

	UFUNCTION()
		void CreateResourceManager();

	UFUNCTION()
		bool CheckChapterClear();

	UFUNCTION()
		void MoveChapter();

	UFUNCTION(BlueprintCallable)
		void UpdateMapUI();

	UFUNCTION()
		bool IsChapterClear();

	UFUNCTION()
		bool IsGameClear() { return (ChapterLV >= MAX_CHAPTER_COUNT) ? true : false; }

	UFUNCTION(BlueprintCallable)
		void SetCurrentStage(class AStageData* Target) { CurrentStage = Target; };

	UFUNCTION(BlueprintCallable)
		UStageGenerator* GetStageGenerator() { return StageGenerator; }

	UFUNCTION(BlueprintCallable)
		UTransitionManager* GetTransitionManager() { return TransitionManager; }

	UFUNCTION(BlueprintCallable)
		AResourceManager* GetResourceManager() { return ResourceManager; }

	UFUNCTION(BlueprintCallable)
		int32 GetChapterLV() { return ChapterLV; }

	UFUNCTION(BlueprintCallable)
		AStageData* GetCurrentStage() { return CurrentStage; }

	UFUNCTION(BlueprintCallable)
		AStageData* GetLobbyStage() { return LobbyStage; }

	UFUNCTION(BlueprintCallable)
		TArray<class AStageData*> GetStages() { return StageList; }
	
private:

	UFUNCTION()
		void InitChapterManager();

	UFUNCTION()
		void CreateChapter();

	UFUNCTION()
		void InitStartGate();

private:
	UPROPERTY(VisibleAnywhere)
		int32 ChapterLV;

	UPROPERTY(VisibleAnywhere)
		float StatWeight;

private:
	UPROPERTY()
	TArray<class AStageData*> StageList;

	UPROPERTY()
		class AStageData* CurrentStage;

	UPROPERTY()
		class AStageData* LobbyStage;
private:
	UPROPERTY()
		class UStageGenerator* StageGenerator;

	UPROPERTY()
		class UTransitionManager* TransitionManager;

	UPROPERTY()
		class AResourceManager* ResourceManager;


};
