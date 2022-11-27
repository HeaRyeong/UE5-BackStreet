// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATile::InitTile(int _x, int _y)
{
	x = _x;
	y = _y;
	GateCnt = 0;

	up = down = left = right = false;

}

bool ATile::IsVisited()
{
	return up || down || left || right;
}

void ATile::GateCount()
{
	if (up)
		GateCnt++;
	if (down)
		GateCnt++;
	if (left)
		GateCnt++;
	if (right)
		GateCnt++;
}
