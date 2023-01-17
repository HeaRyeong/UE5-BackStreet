// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Item/public/ItemBase.h"
#include "../public/MissionBase.h"
#include "UObject/SoftObjectPath.h"
#include "Kismet/GameplayStatics.h"


ATileBase::ATileBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called every frame
void ATileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileBase::BeginPlay()
{
	Super::BeginPlay();

}


void ATileBase::InitTile(int XPosition, int YPosition)
{
	XPos = XPosition;
	YPos = YPosition;

	bIsSpawned=bIsMissionClear = bIsBossStage = bIsMainMission = bIsClear = false;
	Gate.Add(false); // UP
	Gate.Add(false); // DOWN
	Gate.Add(false); // LEFT
	Gate.Add(false); // RIGHT


	StageLevel = FMath::RandRange(1, 5);
	
}

bool ATileBase::IsVisited()
{
	return Gate[static_cast<int>(EDirection::E_UP)] || Gate[static_cast<int>(EDirection::E_DOWN)] || Gate[static_cast<int>(EDirection::E_LEFT)] || Gate[static_cast<int>(EDirection::E_RIGHT)];
}


void ATileBase::SelectMap()
{
	int NextMap = FMath::RandRange(1, 3);
	// 메인, 서브, 보스에 따라 또 갈리기
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

void ATileBase::InitMission(bool IsBoss)
{
	bIsMainMission = true;
	bIsBossStage = IsBoss;
}

void ATileBase::LoadLevel()
{
	ABackStreetGameModeBase* gamemode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LevelRef != nullptr) // 레벨 스트리밍 인스턴스 존재
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));
		LevelRef->SetShouldBeLoaded(true);
		LevelRef->SetShouldBeVisible(true);

		// 플레이어 위치 스폰 위치 수정 필요
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

		// 플레이어 위치 스폰 위치 수정 필요
		ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		myCharacter->SetActorLocation(this->GetActorLocation()+FVector(0,0,500));
		
	}
}

void ATileBase::UnLoadLevel()
{
	if (LevelRef != nullptr) // 레벨 스트리밍 인스턴스 존재
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Now UnLoad Level"));
		LevelRef->SetShouldBeLoaded(false);
		LevelRef->SetShouldBeVisible(false);
		bIsSpawned = true;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , error"));

	}

}

void ATileBase::ClearCheck()
{
	if (MonsterList.IsEmpty())
	{
		bIsClear = true;

		/*ABackStreetGameModeBase* gamemode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gamemode->StageLevel < 6)
		{
			gamemode->StageLevel++;
		}*/
	}
}


void ATileBase::LoadMonsterAsset()
{
	UE_LOG(LogTemp, Log, TEXT("Spawn Monster Point : %d"), MonsterSpawnPoints.Num());

	if (MonsterAssets.Num() > 0)
	{
		ABackStreetGameModeBase* GameMode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		AssetStreamingHandle = GameMode->StreamableManager.RequestAsyncLoad(MonsterAssets, FStreamableDelegate::CreateUObject(this, &ATileBase::SpawnMonster));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("MonsterAssets is Empty"));

	}
}

void ATileBase::SpawnMonster()
{
	int32 MaxSpawn = 5;
	UE_LOG(LogTemp, Log, TEXT("SpawnMonster Func Call"));

	if(!bIsSpawned)
	{
		for (int i = 0; i < 100; i++)
		{
			int32 SelectidxA = FMath::RandRange(0, MonsterSpawnPoints.Num() - 1);
			int32 SelectidxB = FMath::RandRange(0, MonsterSpawnPoints.Num() - 1);
			AActor* Temp;

			Temp = MonsterSpawnPoints[SelectidxA];
			MonsterSpawnPoints[SelectidxA] = MonsterSpawnPoints[SelectidxB];
			MonsterSpawnPoints[SelectidxB] = Temp;

		}

		for (int i = 0; i < MaxSpawn; i++)
		{

			if (MonsterAssets[0].IsValid())
			{
				if (MonsterAssets[0].ResolveObject())
				{
					UBlueprint* Gen = Cast<UBlueprint>(MonsterAssets[0].ResolveObject());
					MonsterList.AddUnique(GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, MonsterSpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator));
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("Wrong"));
				}


			}

		}
	}


}

void ATileBase::LoadItemAsset()
{
	UE_LOG(LogTemp, Log, TEXT("Spawn Item Point : %d"), ItemSpawnPoints.Num());

	if (ItemAssets.Num() > 0)
	{
		ABackStreetGameModeBase* GameMode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		AssetStreamingHandle = GameMode->StreamableManager.RequestAsyncLoad(ItemAssets, FStreamableDelegate::CreateUObject(this, &ATileBase::SpawnItem));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ItemAssets is Empty"));

	}
}

void ATileBase::SpawnItem()
{
	if (!bIsSpawned)
	{
		int32 MaxSpawn = 1;
		UE_LOG(LogTemp, Log, TEXT("SpawnItem Func Call"));
		for (int i = 0; i < 100; i++)
		{
			int32 SelectidxA = FMath::RandRange(0, ItemSpawnPoints.Num() - 1);
			int32 SelectidxB = FMath::RandRange(0, ItemSpawnPoints.Num() - 1);
			AActor* Temp;

			Temp = ItemSpawnPoints[SelectidxA];
			ItemSpawnPoints[SelectidxA] = ItemSpawnPoints[SelectidxB];
			ItemSpawnPoints[SelectidxB] = Temp;

		}

		for (int i = 0; i < MaxSpawn; i++)
		{

			if (ItemAssets[0].IsValid())
			{
				UBlueprint* Gen = Cast<UBlueprint>(ItemAssets[0].ResolveObject());
				GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, ItemSpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);

			}

		}
	}
}

void ATileBase::SetMission()
{
	MissionInfo = Cast<AMissionBase>(GetWorld()->SpawnActor<AMissionBase>(this->GetActorLocation(), FRotator::ZeroRotator));
}



void ATileBase::LoadMissionAsset()
{
	UE_LOG(LogTemp, Log, TEXT("Spawn Mission Point : %d"), MissionSpawnPoints.Num());

	if (MissionAssets.Num() > 0)
	{
		ABackStreetGameModeBase* GameMode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		AssetStreamingHandle = GameMode->StreamableManager.RequestAsyncLoad(MissionAssets, FStreamableDelegate::CreateUObject(this, &ATileBase::SpawnMission));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Mission Assets is Empty"));

	}
}

void ATileBase::SpawnMission()
{
	if (!bIsSpawned)
	{
		int32 MaxSpawn = 1;
		UE_LOG(LogTemp, Log, TEXT("SpawnMission Func Call"));
		for (int i = 0; i < 100; i++)
		{
			int32 SelectidxA = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
			int32 SelectidxB = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
			AActor* Temp;

			Temp = MissionSpawnPoints[SelectidxA];
			MissionSpawnPoints[SelectidxA] = MissionSpawnPoints[SelectidxB];
			MissionSpawnPoints[SelectidxB] = Temp;

		}

		for (int i = 0; i < MaxSpawn; i++)
		{
			if (MissionInfo->Type == 0)
			{
				if (MissionAssets[0].IsValid())
				{
					UBlueprint* Gen = Cast<UBlueprint>(MissionAssets[0].ResolveObject());
					GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, MissionSpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);

				}

			}
			else
			{
				if (MissionAssets[1].IsValid())
				{
					UBlueprint* Gen = Cast<UBlueprint>(MissionAssets[1].ResolveObject());
					GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, MissionSpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);

				}

			}


		}
	}

}

