// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BackStreet.h"
#include "GameFramework/GameModeBase.h"
#include "DirectionEnumInfo.h"
#include "BackStreetGameModeBase.generated.h"


UCLASS()
class BACKSTREET_API ABackStreetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		class UDataTable* StageDataTable;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AGrid* Map;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* CurrTile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int StageLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 PreDir = (uint8)(EDirection::E_DOWN); // 다음 스테이지 기준으로 입구 게이트

public:
	ABackStreetGameModeBase();
	UFUNCTION(BlueprintCallable)
		void InitGame();
	UFUNCTION(BlueprintCallable)
		void NextStage(uint8 Dir);
	UFUNCTION(BlueprintCallable)
		void MoveTile(uint8 NextDir);

};
