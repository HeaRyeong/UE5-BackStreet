// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChapterManagerBase.generated.h"
// Chapter °¹¼ö

#define ChapterNumber 2 
#define MaxMission 4

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
		bool IsGameClear() { return (ChapterLV > ChapterNumber) ? true : false; }

	UFUNCTION()
		bool IsChapterClear() { return Missions.IsEmpty(); }

	//UFUNCTION()
	//	void ClearChapter();

	UFUNCTION()
		void RemoveMissionItem(class AItemBase* target);

	UFUNCTION()
		void RemoveMission(class UMissionBase* target);

	UFUNCTION()
		AGridBase* GetChapter() { return CurrentChapter; }

	UFUNCTION()
		float GetChapterWeight() { return StatWeight; }

	UFUNCTION()
		int8 GetChapterLV() { return ChapterLV; }

	UFUNCTION()
		AStageManagerBase* GetStageManager() { return StageManager; }

private:
	UFUNCTION()
		void CreateChapter();

	UFUNCTION()
		void CreateMission();

	UFUNCTION()
		void CleanChapterManager();

private:
	UPROPERTY()
		class AGridBase* CurrentChapter;

	UPROPERTY()
		int8 ChapterLV;

	UPROPERTY()
		float StatWeight;

private:
	UPROPERTY()
		class AStageManagerBase* StageManager;

	UPROPERTY()
		TArray<class UMissionBase*> Missions;

};
