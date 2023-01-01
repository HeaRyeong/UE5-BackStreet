// Fill out your copyright notice in the Description page of Project Settings.
#include "Tile.h"
#include "EngineUtils.h"
#include "../public/BackStreetGameModeBase.h"
#include "Kismet/GameplayStatics.h"


ATile::ATile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ATile::InitTile(int _x, int _y)
{
	x = _x;
	y = _y;

	IsClear = false;
	Gate.Add(false); // UP
	Gate.Add(false); // DOWN
	Gate.Add(false); // LEFT
	Gate.Add(false); // RIGHT

	UE_LOG(LogTemp, Log, TEXT("Tile location %s %d %d"), *this->GetActorLocation().ToString(), x, y);

	int NextMap = FMath::RandRange(1, 3);

	switch (NextMap)
	{
	case 1:
		LevelToLoad = FName(TEXT("Main1"));
		break;
	case 2:
		LevelToLoad = FName(TEXT("Main2"));
		break;
	case 3:
		LevelToLoad = FName(TEXT("Main3"));
		break;
		//case 4:
		//	NextLevelToLoad = FName(TEXT("Sub1Prefab"));
		//	break;
		//case 5:
		//	NextLevelToLoad = FName(TEXT("Sub2Prefab"));
		//	break;
		//case 6:
		//	NextLevelToLoad = FName(TEXT("Sub3Prefab"));
		//	break;
	}

}

bool ATile::IsVisited()
{
	return Gate[static_cast<int>(EDirection::E_UP)] || Gate[static_cast<int>(EDirection::E_DOWN)] || Gate[static_cast<int>(EDirection::E_LEFT)] || Gate[static_cast<int>(EDirection::E_RIGHT)];
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::CountGate()
{
	GateCnt = 0;
	if (Gate[(uint8)(EDirection::E_UP)])
		GateCnt++;
	if (Gate[(uint8)(EDirection::E_DOWN)])
		GateCnt++;
	if (Gate[(uint8)(EDirection::E_LEFT)])
		GateCnt++;
	if (Gate[(uint8)(EDirection::E_RIGHT)])
		GateCnt++;
}

void ATile::ClearCheck()
{
	if (MonsterList.IsEmpty())
	{
		IsClear = true;

		ABackStreetGameModeBase* gamemode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gamemode->StageLevel < 6)
		{
			gamemode->StageLevel++;
		}
	}
}