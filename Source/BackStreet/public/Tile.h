// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class BACKSTREET_API ATile : public AActor
{
	GENERATED_BODY()

public:
	char stagemap;
	int x;
	int y;
	int GateCnt;

public:
	bool up, down, left, right;

public:
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void InitTile(int width, int hight);
	bool IsVisited();
	void GateCount();
};
