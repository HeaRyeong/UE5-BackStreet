// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "MissionBase.generated.h"

UCLASS()
class BACKSTREET_API AMissionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MonsterSpawnPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> ItemSpawnPoints;
};
