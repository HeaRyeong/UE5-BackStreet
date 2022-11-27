// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../public/LevelStreamerActor.h"
#include "../public/Grid.h"
#include "BackStreetGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALevelStreamerActor* CurrentVolume;
	ALevelStreamerActor* StreamerVolumeA;
	ALevelStreamerActor* StreamerVolumeB;

public:
	UPROPERTY(EditAnywhere)
		FName NextLevelToLoad;
	UPROPERTY(EditAnywhere)
		FVector CurrentTransform;
	UPROPERTY(EditAnywhere)
		FVector PreTransform;
	UPROPERTY(EditAnywhere)
		FVector CurrentVolumeLocation;
	FGrid Map;

public:
	UFUNCTION(BlueprintCallable)
		void InitGame();
	UFUNCTION(BlueprintCallable)
		void SelectRandomMap();
	UFUNCTION(BlueprintCallable)
		void SetMap();
	UFUNCTION(BlueprintCallable)
		void StageClear();
	UFUNCTION(BlueprintCallable)
		void GateOpen();
	UFUNCTION(BlueprintCallable)
		void SetVolume();
	void PrintTileInfo();

};
