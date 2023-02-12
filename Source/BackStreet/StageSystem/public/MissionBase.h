// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../Global/public/BackStreet.h"
#include "MissionBase.generated.h"

UCLASS()
class BACKSTREET_API UMissionBase : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UMissionBase();

public:
	UFUNCTION()
		void InitMission(class ATileBase* tile, int8 type);

	UFUNCTION()
		bool ClearCheck();

	UFUNCTION()
		void RemoveItem(class AItemBase* target);

	UFUNCTION()
		void RemoveMonster(class AEnemyCharacterBase* target);
	
public:
	// 1 or 2 or 3, 1 == Item 습득 미션, 2 == 몬스터 잡기 미션 , 3 == 보스
	UPROPERTY()
		int8 Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AEnemyCharacterBase*> MonsterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AItemBase*> ItemList;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* BelongTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABackStreetGameModeBase* GameModeRef;

};
