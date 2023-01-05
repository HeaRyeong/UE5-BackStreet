// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BackStreet.h"
#include "GameFramework/GameModeBase.h"
#include "BackStreetGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius = 100.0f);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		TArray<TSubclassOf<UCameraShakeBase>> CameraShakeEffectList;

};
