// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "Engine/LevelScriptActor.h"
#include "StageLevelScriptBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AStageLevelScriptBase : public ALevelScriptActor
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION()
		void LoadSpawnPoint();

protected:
	UPROPERTY(BlueprintReadOnly)
		class ABackStreetGameModeBase* GamemodeRef;
};
