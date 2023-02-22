// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
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

	UFUNCTION()
		void TeleportCharacter();
public:
	UFUNCTION()
		void SetGate();

	UFUNCTION()
		void SetSpawnPoints(class ATileBase* target);



private:
	UPROPERTY(VisibleAnywhere)
		class ATileBase* BelongTileRef;

	// Sequence

public:
	UFUNCTION()
		void PlayLoadSequencePlayer();

private:
	UFUNCTION()
		void InitLevelSequence();

	UFUNCTION()
		void ClearAllTimerHandle();

private:
	UPROPERTY(VisibleAnywhere)
		class ULevelSequencePlayer* LoadSequencePlayer;

	UPROPERTY(VisibleAnywhere)
		FTimerHandle TravelSequenceDelayHandle;

	UPROPERTY(VisibleAnywhere)
		FTimerHandle ResourceReturnTimerHandle;

private:
	UPROPERTY()
		class ALevelScriptInGame* InGameScriptRef;

	UPROPERTY()
		class ABackStreetGameModeBase* GameModeRef;
	
};
