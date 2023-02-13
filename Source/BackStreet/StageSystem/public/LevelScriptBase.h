// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScriptBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API ALevelScriptBase : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	ALevelScriptBase();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void InitLevel(class ATileBase* target);

private:
	UFUNCTION()
		void SetGate();

	UFUNCTION()
		void SetSpawnPoints(class ATileBase* target);

private:
	UPROPERTY()
		class ALevelScriptInGame* InGameScriptRef;
	
};
