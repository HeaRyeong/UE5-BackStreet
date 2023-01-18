// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../public/BackStreet.h"
#include "../../StageSystem/public/DirectionEnumInfo.h"
#include "GameFramework/GameModeBase.h"
#include "BackStreetGameModeBase.generated.h"


UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABackStreetGameModeBase();

public:
	UFUNCTION(BlueprintCallable)
		void InitGame();

	UFUNCTION(BlueprintCallable)
		void InitChapter();

	UFUNCTION(BlueprintCallable)
		void NextStage(uint8 Dir);

	UFUNCTION(BlueprintCallable)
		void MoveTile(uint8 NextDir);

	UFUNCTION()
		void PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius = 100.0f);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AGrid* Chapter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* CurrTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RemainChapter; // ���� é�� �� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 PreDir = (uint8)(EDirection::E_DOWN); // ���� �������� �������� �Ա� ����Ʈ

	UPROPERTY(BlueprintReadWrite)
		bool bIsGamePaused = false;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		TArray<TSubclassOf<UCameraShakeBase>> CameraShakeEffectList;

};
