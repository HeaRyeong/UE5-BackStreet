// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../public/MissionBase.h"
#include "../../Item/public/ItemInfoStruct.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Item/public/ProjectileBase.h"
#include "../../Item/public/ItemBoxBase.h"
#include "../public/ChapterManagerBase.h"
#include "TimerManager.h"
#include "../../AISystem/public/AIControllerBase.h"
#include "../public/GridBase.h"
#include "../public/ALevelScriptInGame.h"
#include "../public/LevelScriptBase.h"
#include "UObject/SoftObjectPath.h"
#include "../public/RewardBox.h"
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
	CharacterRef = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
	
}

void ATileBase::InitTile(int XPosition, int YPosition)
{
	XPos = XPosition;
	YPos = YPosition;

	bIsVisited = bIsClear = false;
	Gate.Add(false); // UP
	Gate.Add(false); // DOWN
	Gate.Add(false); // LEFT
	Gate.Add(false); // RIGHT

	StageType = EStageCategoryInfo::E_Normal;
	StageTime = 0;
	
	SelectMap();

	// Timer 설정뒤 Pause
	GameModeRef->ClearResourceDelegate.AddDynamic(this, &ATileBase::ClearTimer);
	GetWorldTimerManager().SetTimer(StageTimerHandle, this, &ATileBase::AddTime, 1.0f, true);
	GetWorldTimerManager().PauseTimer(StageTimerHandle);
}

void ATileBase::SelectMap()
{

	const TArray<FName> Stages = { FName("TypeA"),FName("TypeB"),FName("TypeC"),FName("TypeD"),FName("TypeE"),FName("TypeF") };
	int32 mapIdx = FMath::RandRange(0, Stages.Num() - 1);

	LevelToLoad = Stages[mapIdx];
}

bool ATileBase::IsVisited()
{
	return Gate[static_cast<int>(EDirection::E_UP)] || Gate[static_cast<int>(EDirection::E_DOWN)] || Gate[static_cast<int>(EDirection::E_LEFT)] || Gate[static_cast<int>(EDirection::E_RIGHT)];
}

void ATileBase::SetStage()
{
	if (StageType == EStageCategoryInfo::E_Boss)
	{
		SpawnMission();
		SpawnItem();
	}
	else if (StageType == EStageCategoryInfo::E_Mission)
	{
		SpawnMonster();
		SpawnItem();
		SpawnMission();
	}
	else
	{
		SpawnMonster();
		SpawnItem();
	}

	BindDelegate();
	UE_LOG(LogTemp, Log, TEXT("finish spawn"));

}

void ATileBase::SpawnMonster()
{
	uint16 type = FMath::RandRange(0, MAX_STAGE_TYPE - 1);
	FStageEnemyTypeStruct stageTypeInfo = GameModeRef->GetStageTypeInfoWithRow(type);
	TArray<int32> enemyIDList;
	int8 spawnNum = FMath::RandRange(stageTypeInfo.MinSpawn, stageTypeInfo.MaxSpawn);

	for (int i = 0; i < 100; i++)
	{
		int32 selectidxA = FMath::RandRange(0, MonsterSpawnPoints.Num() - 1);
		int32 selectidxB = FMath::RandRange(0, MonsterSpawnPoints.Num() - 1);
		AActor* temp;

		temp = MonsterSpawnPoints[selectidxA];
		MonsterSpawnPoints[selectidxA] = MonsterSpawnPoints[selectidxB];
		MonsterSpawnPoints[selectidxB] = temp;

	}


	if (stageTypeInfo.ID_1001)
	{
		enemyIDList.AddUnique(1001);

	}
	if (stageTypeInfo.ID_1002)
	{
		enemyIDList.AddUnique(1002);

	}
	if (stageTypeInfo.ID_1003)
	{
		enemyIDList.AddUnique(1003);

	}
	if (stageTypeInfo.ID_1100)
	{
		enemyIDList.AddUnique(1100);

	}
	if (stageTypeInfo.ID_1101)
	{
		enemyIDList.AddUnique(1101);

	}
	if (stageTypeInfo.ID_1102)
	{
		enemyIDList.AddUnique(1102);
	}

	for (int32 i = 0; i < spawnNum; i++)
	{
		int32 enemyIDIdx = FMath::RandRange(0, enemyIDList.Num() - 1);
	
		FActorSpawnParameters actorSpawnParameters;
		actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyCharacterBase* target = GetWorld()->SpawnActor<AEnemyCharacterBase>(InGameScriptRef->GetAssetManager()->GetEnemyWithID(enemyIDList[enemyIDIdx]), MonsterSpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator, actorSpawnParameters);
		MonsterList.Add(target);
		target->EnemyID = enemyIDList[enemyIDIdx];
		target->InitEnemyStat();
	}
	
}

void ATileBase::SpawnItem()
{
	//스폰 포인트 순서를 무작위로 지정
	for (int i = 0; i < 100; i++)
	{
		int32 selectidxA = FMath::RandRange(0, ItemSpawnPoints.Num() - 1);
		int32 selectidxB = FMath::RandRange(0, ItemSpawnPoints.Num() - 1);
		AActor* temp;

		temp = ItemSpawnPoints[selectidxA];
		ItemSpawnPoints[selectidxA] = ItemSpawnPoints[selectidxB];
		ItemSpawnPoints[selectidxB] = temp;

	}

	int8 spawnMax = FMath::RandRange(MIN_ITEM_SPAWN, MAX_ITEM_SPAWN);
	for (int8 i = 0; i < spawnMax; i++)
	{
		int8 type = FMath::RandRange(0, InGameScriptRef->GetAssetManager()->ItemBoxAssets.Num() - 1);
		AItemBoxBase* target = GetWorld()->SpawnActor<AItemBoxBase>(InGameScriptRef->GetAssetManager()->ItemBoxAssets[type]
																	, ItemSpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
		target->InitItemBox(false);
		ItemBoxList.AddUnique(target);
	}
}

void ATileBase::SpawnMission()
{
	for (int i = 0; i < 100; i++)
	{
		int32 selectidxA = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
		int32 selectidxB = FMath::RandRange(0, MissionSpawnPoints.Num() - 1);
		AActor* newMissionItemBox; //미션 아이템이 포함된 아이템 박스

		newMissionItemBox = MissionSpawnPoints[selectidxA];
		MissionSpawnPoints[selectidxA] = MissionSpawnPoints[selectidxB];
		MissionSpawnPoints[selectidxB] = newMissionItemBox;
		
	}

	if(Mission->Type == 1) // 아이템 습득
	{
		FActorSpawnParameters actorSpawnParameters;
		actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
		AItemBoxBase* target = GetWorld()->SpawnActor<AItemBoxBase>(InGameScriptRef->GetAssetManager()->MissionAssets[0], MissionSpawnPoints[0]->GetActorLocation()
																	, FRotator::ZeroRotator, actorSpawnParameters);
		target->InitItemBox(true);
		ItemBoxList.AddUnique(target);
		target->OnMissionItemSpawned.BindUFunction(Mission, FName("TryAddMissionItem"));
	}
	else if(Mission->Type == 2)// 몬스터 잡기
	{
		FActorSpawnParameters actorSpawnParameters;
		actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyCharacterBase* target = GetWorld()->SpawnActor<AEnemyCharacterBase>(InGameScriptRef->GetAssetManager()->MissionEnemyAssets[0], MissionSpawnPoints[0]->GetActorLocation(), FRotator::ZeroRotator, actorSpawnParameters);
		Mission->MonsterList.Add(target);
		target->Tags.AddUnique("MissionMonster");
		MonsterList.AddUnique(target);
		Mission->MonsterList.AddUnique(target);
		
	}
	else // 보스
	{
		FActorSpawnParameters actorSpawnParameters;
		actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyCharacterBase* target = GetWorld()->SpawnActor<AEnemyCharacterBase>(InGameScriptRef->GetAssetManager()->MissionEnemyAssets[1], MissionSpawnPoints[0]->GetActorLocation(), FRotator::ZeroRotator, actorSpawnParameters);
		Mission->MonsterList.Add(target);
		target->Tags.AddUnique("MissionMonster");
		MonsterList.AddUnique(target);
		Mission->MonsterList.AddUnique(target);
		
	}		
}

void ATileBase::SpawnRewardBox()
{
	FActorSpawnParameters actorSpawnParameters;
	actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ARewardBox* target = GetWorld()->SpawnActor<ARewardBox>(InGameScriptRef->GetAssetManager()->RewardBoxAssets[0], RewardBoxSpawnPoint[0]->GetActorLocation(), FRotator(0,90,0), actorSpawnParameters);
	target->Tags.AddUnique("RewardBox");
}

void ATileBase::MonsterDie(AEnemyCharacterBase* target)
{
	UE_LOG(LogTemp, Log, TEXT("Call MonsterDie()"));

	if (target->ActorHasTag("MissionMonster"))
	{
		Mission->TryRemoveMonster(target);
	}

	MonsterList.Remove(target);

	if (MonsterList.IsEmpty())
	{
		bIsClear = true;

		//// 스테이지 클리어 처리
		//GamemodeRef->FinishChapterDelegate.Broadcast(false);
		StageClearDelegate.Broadcast();
		SpawnRewardBox();
		PauseTimer();
	}
}

void ATileBase::BindDelegate()
{
	for (AEnemyCharacterBase* enemy : MonsterList)
	{
		FString name = enemy->GetController()->GetName();
		FinishTileDelegate.AddDynamic(Cast<AAIControllerBase>(enemy->GetController()), &AAIControllerBase::DeactivateAI);
		StartTileDelegate.AddDynamic(Cast<AAIControllerBase>(enemy->GetController()), &AAIControllerBase::ActivateAI);
		enemy->EnemyDeathDelegate.BindUFunction(this, FName("MonsterDie"));
	}
}

void ATileBase::UnPauseStage()
{
	if (StartTileDelegate.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("Call UnPauseStage"));
		StartTileDelegate.Broadcast();
		UnPauseTimer();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Please Check Binding"));
	}
}

void ATileBase::PauseStage()
{
	if (FinishTileDelegate.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("Call PauseStage"));
		FinishTileDelegate.Broadcast();
		PauseTimer();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Please Check Binding"));
	}
}


void ATileBase::StageReward()
{
	/*
	EStatUpCategoryInfo Type = (EStatUpCategoryInfo)FMath::RandRange(1, 5);
	FCharacterStatStruct NewStat = CharacterRef->GetCharacterStat();
	float RewardValue;

	if (ClearTime < 1) // A등급
	{
		RewardValue = GamemodeRef->ChapterStatValue + 0.3f;
		UE_LOG(LogTemp, Log, TEXT("A Rank %f"),RewardValue);
	}
	else if (ClearTime < 3) // B등급
	{
		RewardValue = GamemodeRef->ChapterStatValue + 0.2f;
		UE_LOG(LogTemp, Log, TEXT("B Rank %f"), RewardValue);
	}
	else // C등급
	{
		RewardValue = GamemodeRef->ChapterStatValue + 0.1f;
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

	CharacterRef->UpdateCharacterStat(NewStat);*/
}

void ATileBase::AddTime()
{
	StageTime++;
}


void ATileBase::PauseTimer()
{
	UE_LOG(LogTemp, Log, TEXT("Call PauseTiemr"));
	GetWorldTimerManager().PauseTimer(StageTimerHandle);
}

void ATileBase::UnPauseTimer()
{
	UE_LOG(LogTemp, Log, TEXT("Call UnPauseTiemr"));

	if(!bIsClear)
		GetWorldTimerManager().UnPauseTimer(StageTimerHandle);
}

void ATileBase::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	//GetWorldTimerManager().ClearTimer(StageTimerHandle);
}
