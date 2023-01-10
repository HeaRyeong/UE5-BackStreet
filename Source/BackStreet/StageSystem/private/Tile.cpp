// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/Tile.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"


ATile::ATile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ATile::InitTile(int XPosition, int YPosition)
{
	XPos = XPosition;
	YPos = YPosition;

	bIsMissionClear = bIsBossStage = bIsMainMission = bIsClear = false;
	Gate.Add(false); // UP
	Gate.Add(false); // DOWN
	Gate.Add(false); // LEFT
	Gate.Add(false); // RIGHT


	StageLevel = FMath::RandRange(1, 5);
	
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

void ATile::ClearCheck()
{
	/*if (MonsterList.IsEmpty())
	{
		IsClear = true;

		ABackStreetGameModeBase* gamemode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gamemode->StageLevel < 6)
		{
			gamemode->StageLevel++;
		}
	}*/
}

void ATile::LoadLevel()
{
	ABackStreetGameModeBase* gamemode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LevelRef != nullptr) // ���� ��Ʈ���� �ν��Ͻ� ����
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));
		LevelRef->SetShouldBeLoaded(true);
		LevelRef->SetShouldBeVisible(true);

		// �÷��̾� ��ġ ���� ��ġ ���� �ʿ�
		ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		myCharacter->SetActorLocation(this->GetActorLocation()+ FVector(0, 0, 500));

	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , Create Level Instance"));
		FString name = FString::FromInt(gamemode->RemainChapter);
		name += FString(TEXT("Stage"));
		name += FString::FromInt(YPos * 5 + XPos);
		LevelRef=UGameplayStatics::GetStreamingLevel(GetWorld(), LevelToLoad)->CreateInstance(name);
		LevelRef->LevelTransform.SetLocation(this->GetActorLocation());

		LevelRef->SetShouldBeLoaded(true);
		LevelRef->SetShouldBeVisible(true);

		// �÷��̾� ��ġ ���� ��ġ ���� �ʿ�
		ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		myCharacter->SetActorLocation(this->GetActorLocation()+FVector(0,0,500));
		
	}
}

void ATile::UnLoadLevel()
{
	if (LevelRef != nullptr) // ���� ��Ʈ���� �ν��Ͻ� ����
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Now UnLoad Level"));
		LevelRef->SetShouldBeLoaded(false);
		LevelRef->SetShouldBeVisible(false);

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , error"));

	}

}

void ATile::InitMission(bool IsBoss)
{
	bIsMainMission = true;
	bIsBossStage = IsBoss;

	// �̼� ���� ����?
	// �̼� ���� �� Grid�� �̼� ����Ʈ�� �߰��ϱ�


}

void ATile::SetMission()
{
	// �̼� ���� ������ or �� ����
}

void ATile::SelectMap()
{
	int NextMap = FMath::RandRange(1, 3);
	// ����, ����, ������ ���� �� ������
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