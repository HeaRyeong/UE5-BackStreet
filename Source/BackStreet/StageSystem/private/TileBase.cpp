// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Item/public/ItemBase.h"
#include "../public/MissionBase.h"
#include "../../Item/public/ItemInfoStruct.h"
//#include "../../Character/public/CharacterInfoStruct.h"
#include "TimerManager.h"
#include "../public/GridBase.h"
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
	MyCharacter = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GameMode = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Chapter = GameMode->Chapter;

	SelectMap();
}

void ATileBase::InitTile(int XPosition, int YPosition)
{
	XPos = XPosition;
	YPos = YPosition;

	bIsSpawned = bIsBossStage = bIsMainMission = bIsClear = false;
	Gate.Add(false); // UP
	Gate.Add(false); // DOWN
	Gate.Add(false); // LEFT
	Gate.Add(false); // RIGHT

	StageType = EStageCategoryInfo::E_Normal;
	ClearTime = 0;
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
	if (IsBoss)
	{
		StageType = EStageCategoryInfo::E_Boss;
	}
	else
	{
		StageType = EStageCategoryInfo::E_Normal;
	}
	bIsMainMission = true;
	bIsBossStage = IsBoss;
	MissionInfo = NewObject<UMissionBase>(this);
	MissionInfo->InitMission();
	MissionInfo->Tile = this; 
	Chapter->Missions.AddUnique(MissionInfo);
}

void ATileBase::LoadLevel()
{
	AssetDataManagerRef = GameMode->AssetDataManager;
	if (LevelRef != nullptr) // 레벨 스트리밍 인스턴스 존재
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));
		LevelRef->SetShouldBeLoaded(true);
		LevelRef->SetShouldBeVisible(true);

		// 플레이어 위치 스폰 위치 수정 필요
		MyCharacter->SetActorLocation(this->GetActorLocation()+ FVector(0, 0, 500));
		// Timer
		GetWorldTimerManager().UnPauseTimer(ClearTimerHandle);

	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , Create Level Instance"));
		FString name = FString::FromInt(GameMode->RemainChapter);
		name += FString(TEXT("Stage"));
		name += FString::FromInt(YPos * 5 + XPos);
		LevelRef=UGameplayStatics::GetStreamingLevel(GetWorld(), LevelToLoad)->CreateInstance(name);
		LevelRef->LevelTransform.SetLocation(this->GetActorLocation());

		LevelRef->SetShouldBeLoaded(true);
		LevelRef->SetShouldBeVisible(true);

		// 플레이어 위치 스폰 위치 수정 필요
		MyCharacter->SetActorLocation(this->GetActorLocation()+FVector(0,0,500));
		// Clear Timer 설정
		GetWorldTimerManager().SetTimer(ClearTimerHandle, this, &ATileBase::SetReward, 60.0f, true);

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
		for (AEnemyCharacterBase* Target : MonsterList)
		{
			Target->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
		// Pause Timer
		GetWorldTimerManager().PauseTimer(ClearTimerHandle);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , error"));

	}

}

void ATileBase::LoadMonster()
{
	UE_LOG(LogTemp, Log, TEXT("Spawn Monster Point : %d"), MonsterSpawnPoints.Num());

	if (!bIsSpawned)
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
		AssetDataManagerRef->LoadMonsterAsset(MonsterSpawnPoints, this);
	}

}

void ATileBase::MonsterDie(AEnemyCharacterBase* Target)
{
	UE_LOG(LogTemp, Log, TEXT("Call MonsterDie()"));

	if (Target->ActorHasTag("MissionMonster"))
	{
		MissionInfo->MonsterList.Remove(Target);
		MissionInfo->ClearCheck();
	}

	MonsterList.Remove(Target);

	if (MonsterList.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Stage Clear"));
		bIsClear = true;

		// 스테이지 클리어 보상 처리
		StageReward();
		
	}
}

void ATileBase::BindDelegate()
{
	for (AEnemyCharacterBase* enemy : MonsterList)
	{
		UE_LOG(LogTemp, Log, TEXT("Binding Func"));
		enemy->EnemyDeathDelegate.BindUFunction(this, FName("MonsterDie"));
	}
}

void ATileBase::LoadItem()
{
	UE_LOG(LogTemp, Log, TEXT("Spawn Item Point : %d"), ItemSpawnPoints.Num());

	if (!bIsSpawned)
	{	
		for (int i = 0; i < 100; i++)
		{
			int32 SelectidxA = FMath::RandRange(0, ItemSpawnPoints.Num() - 1);
			int32 SelectidxB = FMath::RandRange(0, ItemSpawnPoints.Num() - 1);
			AActor* Temp;

			Temp = ItemSpawnPoints[SelectidxA];
			ItemSpawnPoints[SelectidxA] = ItemSpawnPoints[SelectidxB];
			ItemSpawnPoints[SelectidxB] = Temp;

		}
		int8 SpawnMax = FMath::RandRange(1, MaxItemSpawn);
		for (int8 i = 0; i < 1; i++)
		{
			int8 ItemType = FMath::RandRange(1, 3);
			AssetDataManagerRef->LoadItemAsset(EItemCategoryInfo(ItemType), ItemSpawnPoints[i]);
		}
		
	}
}




void ATileBase::LoadMissionAsset()
{
	if(!bIsSpawned)
	{	
		for (int i = 0; i < 100; i++)
		{
			int32 SelectidxA = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
			int32 SelectidxB = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
			AActor* Temp;

			Temp = MissionSpawnPoints[SelectidxA];
			MissionSpawnPoints[SelectidxA] = MissionSpawnPoints[SelectidxB];
			MissionSpawnPoints[SelectidxB] = Temp;

		}

		if(MissionInfo->Type == 1) // 아이템 습득
		{
			AssetDataManagerRef->LoadItemAsset(EItemCategoryInfo::E_Mission, MissionSpawnPoints[0]);
		}
		else if(MissionInfo->Type == 2)// 몬스터 잡기
		{
			if (!bIsSpawned)
			{
				for (int i = 0; i < 100; i++)
				{
					int32 SelectidxA = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
					int32 SelectidxB = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
					AActor* Temp;

					Temp = MissionSpawnPoints[SelectidxA];
					MissionSpawnPoints[SelectidxA] = MissionSpawnPoints[SelectidxB];
					MissionSpawnPoints[SelectidxB] = Temp;

				}

			}
			AssetDataManagerRef->LoadMissionMonsterAsset(MissionSpawnPoints, this, MissionInfo);
		}
		else // 보스
		{
			if (!bIsSpawned)
			{
				for (int i = 0; i < 100; i++)
				{
					int32 SelectidxA = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
					int32 SelectidxB = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
					AActor* Temp;

					Temp = MissionSpawnPoints[SelectidxA];
					MissionSpawnPoints[SelectidxA] = MissionSpawnPoints[SelectidxB];
					MissionSpawnPoints[SelectidxB] = Temp;

				}

			}
			AssetDataManagerRef->LoadMissionMonsterAsset(MissionSpawnPoints, this, MissionInfo);
		}
	}
	
}


void ATileBase::StageReward()
{
	EStatUpCategoryInfo Type = (EStatUpCategoryInfo)FMath::RandRange(1, 5);
	FCharacterStatStruct NewStat = MyCharacter->GetCharacterStat();
	float RewardValue;

	if (ClearTime < 1) // A등급
	{
		RewardValue = GameMode->ChapterStatValue + 0.3f;
		UE_LOG(LogTemp, Log, TEXT("A Rank %f"),RewardValue);
	}
	else if (ClearTime < 3) // B등급
	{
		RewardValue = GameMode->ChapterStatValue + 0.2f;
		UE_LOG(LogTemp, Log, TEXT("B Rank %f"), RewardValue);
	}
	else // C등급
	{
		RewardValue = GameMode->ChapterStatValue + 0.1f;
		UE_LOG(LogTemp, Log, TEXT("C Rank %f"), RewardValue);
	}

	switch (Type)
	{
	case EStatUpCategoryInfo::E_None:
		break;
	case EStatUpCategoryInfo::E_MaxHp:
		UE_LOG(LogTemp, Log, TEXT("MaxHp"));
		NewStat.CharacterMaxHP += 0.1f + RewardValue;
		break;
	case EStatUpCategoryInfo::E_ATK:
		UE_LOG(LogTemp, Log, TEXT("ATK"));
		NewStat.CharacterAtkMultiplier += 0.1f + RewardValue;
		break;
	case EStatUpCategoryInfo::E_ATKSpeed:
		UE_LOG(LogTemp, Log, TEXT("ATKSpeed"));
		NewStat.CharacterAtkSpeed += 0.05f + (RewardValue * 0.1);
		break;
	case EStatUpCategoryInfo::E_MoveSpeed:
		UE_LOG(LogTemp, Log, TEXT("MoveSpeed"));
		NewStat.CharacterMoveSpeed += 20.0f + (RewardValue * 10);
		break;
	case EStatUpCategoryInfo::E_Defense:
		UE_LOG(LogTemp, Log, TEXT("Defense"));
		NewStat.CharacterDefense += 0.1f + RewardValue;
		break;
	default:
		break;
	}

	MyCharacter->UpdateCharacterStat(NewStat);
}

void ATileBase::SetReward()
{
	UE_LOG(LogTemp, Log, TEXT("Call SetReward"));
	ClearTime++;
}