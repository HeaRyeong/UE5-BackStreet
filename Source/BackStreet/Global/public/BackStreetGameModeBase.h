// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../public/BackStreet.h"
#include "../../StageSystem/public/DirectionEnumInfo.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/StreamableManager.h"
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
		class AGridBase* Chapter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* CurrTile;
	// 남은 챕터 수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RemainChapter;
	// 다음 스테이지 기준으로 입구 게이트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 PreDir = (uint8)(EDirection::E_DOWN);
	// 애셋 로딩 관련
	FStreamableManager StreamableManager;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		TArray<TSubclassOf<UCameraShakeBase>> CameraShakeEffectList;

};
