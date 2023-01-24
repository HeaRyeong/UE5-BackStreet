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
		void InitMission();
	UFUNCTION()
		void ClearCheck();
	
public:
	// 1 or 2 , 1 == Item ���� �̼�, 2 == ���� ��� �̼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemList;
	
public:
	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* Tile;

};
