// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ChapterManagerBase.generated.h"
// Chapter °¹¼ö

#define MAX_CHAPTER_COUNT 2
#define MAX_MISSION_COUNT 2
#define MAX_MISSION_ITEM_COUNT 10

UCLASS()
class BACKSTREET_API AChapterManagerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChapterManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void InitChapterManager();

	UFUNCTION()
		bool IsGameClear() { return (ChapterLV >= MAX_CHAPTER_COUNT) ? true : false; }

	UFUNCTION()
		bool IsChapterClear() { return Missions.IsEmpty(); }

	UFUNCTION()
		void ClearChapter();

	UFUNCTION()
		bool TryAddMissionItem(AItemBase* target);

	UFUNCTION(BlueprintCallable)
		bool TryRemoveMissionItem(class AItemBase* target);

	UFUNCTION()
		void RemoveMission(class UMissionBase* target);

	UFUNCTION(BlueprintCallable)
		AGridBase* GetChapter() { return CurrentChapter; }

	UFUNCTION()
		float GetChapterWeight() { return StatWeight; }

	UFUNCTION()
		int8 GetChapterLV() { return ChapterLV; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		AStageManagerBase* GetStageManager() { return StageManager; }

private:
	UFUNCTION()
		void CreateChapter();

	UFUNCTION()
		void CreateMission();

	UFUNCTION()
		void CleanChapterManager();

private:
	UPROPERTY(VisibleAnywhere)
		class AGridBase* CurrentChapter;

	UPROPERTY(VisibleAnywhere)
		int8 ChapterLV;

	UPROPERTY(VisibleAnywhere)
		float StatWeight;

private:
	UPROPERTY()
		class ALevelScriptInGame* InGameScriptRef;

	UPROPERTY(VisibleAnywhere)
		class AStageManagerBase* StageManager;

	UPROPERTY(VisibleAnywhere)
		TArray<class UMissionBase*> Missions;

	UPROPERTY()
		class ABackStreetGameModeBase* GameModeRef;

};
