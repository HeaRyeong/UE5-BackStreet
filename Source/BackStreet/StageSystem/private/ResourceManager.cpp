// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ResourceManager.h"
#include "../public/StageData.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Global/public/DebuffManager.h"
#include "../public/ChapterManagerBase.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "../../AISystem/public/AIControllerBase.h"
#include "../../Item/public/RewardBoxBase.h"
#include "../../Item/public/ItemBoxBase.h"
#include "../../Item/public/ItemBase.h"
#include "Engine/LevelStreaming.h"

AResourceManager::AResourceManager()
{
}

void AResourceManager::BeginPlay()
{
	Super::BeginPlay();

	ABackStreetGameModeBase* gameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gameModeRef->ClearResourceDelegate.AddDynamic(this, &AResourceManager::CleanAllResource);
	gameModeRef->ChapterClearResourceDelegate.AddDynamic(this, &AResourceManager::CleanAllResource);
}

void AResourceManager::InitResourceManager()
{
	
}

void AResourceManager::SpawnStageActor(class AStageData* Target)
{
	UE_LOG(LogTemp, Log, TEXT("AResourceManager::SpawnStageActor -> Start Spawn"));
	if (Target->GetStageType() == EStageCategoryInfo::E_Normal)
	{
		SpawnMonster(Target);
		SpawnItem(Target);
	}
	else if (Target->GetStageType() == EStageCategoryInfo::E_Boss)
	{
		SpawnBossMonster(Target);
		SpawnItem(Target);
	}
	BindDelegate(Target);
	UE_LOG(LogTemp, Log, TEXT("AResourceManager::SpawnStageActor -> Finish Spawn"));

}

void AResourceManager::SpawnMonster(class AStageData* Target)
{
	uint16 type = FMath::RandRange(0, MAX_STAGE_TYPE - 1);
	FStageEnemyTypeStruct stageTypeInfo = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStageTypeInfoWithRow(type);
	TArray<int32> enemyIDList = stageTypeInfo.IDList;
	int8 spawnNum = FMath::RandRange(stageTypeInfo.MinSpawn, stageTypeInfo.MaxSpawn);
	TArray<FVector> monsterSpawnPoint = Target->GetMonsterSpawnPoints();

	for (int i = 0; i < 100; i++)
	{
		int32 selectidxA = FMath::RandRange(0, monsterSpawnPoint.Num() - 1);
		int32 selectidxB = FMath::RandRange(0, monsterSpawnPoint.Num() - 1);
		FVector temp;

		temp = monsterSpawnPoint[selectidxA];
		monsterSpawnPoint[selectidxA] = monsterSpawnPoint[selectidxB];
		monsterSpawnPoint[selectidxB] = temp;

	}


	for (int32 i = 0; i < spawnNum; i++)
	{
		int32 enemyIDIdx = FMath::RandRange(0, enemyIDList.Num() - 1);

		FActorSpawnParameters actorSpawnParameters;
		FVector spawnLocation = monsterSpawnPoint[i];
		spawnLocation = spawnLocation + FVector(0, 0, 200);

		actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyCharacterBase* monster = GetWorld()->SpawnActor<AEnemyCharacterBase>(GetEnemyWithID(enemyIDList[enemyIDIdx]),spawnLocation, FRotator::ZeroRotator, actorSpawnParameters);
		Target->AddMonsterList(monster);
		monster->EnemyID = enemyIDList[enemyIDIdx];
		monster->InitEnemyStat();
	}

}

void AResourceManager::SpawnBossMonster(class AStageData* Target)
{
	TArray<FVector> monsterSpawnPoint = Target->GetMonsterSpawnPoints();
	FActorSpawnParameters actorSpawnParameters;
	uint16 idx = FMath::RandRange(0, monsterSpawnPoint.Num() - 1);
	FVector spawnLocation = monsterSpawnPoint[idx];
	spawnLocation = spawnLocation + FVector(0, 0, 200);

	actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AEnemyCharacterBase* monster = GetWorld()->SpawnActor<AEnemyCharacterBase>(BossAssets[0], spawnLocation, FRotator::ZeroRotator, actorSpawnParameters);
	Target->AddMonsterList(monster);
	// ���� �ʿ� �ϵ��ڵ�..
	monster->EnemyID = 1200;
	monster->InitEnemyStat();
}

void AResourceManager::SpawnItem(class AStageData* Target)
{
	TArray<FVector> itemSpawnPoints = Target->GetItemSpawnPoints();
	for (int i = 0; i < 100; i++)
	{
		int32 selectidxA = FMath::RandRange(0, itemSpawnPoints.Num() - 1);
		int32 selectidxB = FMath::RandRange(0, itemSpawnPoints.Num() - 1);
		FVector temp;

		temp = itemSpawnPoints[selectidxA];
		itemSpawnPoints[selectidxA] = itemSpawnPoints[selectidxB];
		itemSpawnPoints[selectidxB] = temp;

	}

	int8 spawnMax = FMath::RandRange(MIN_ITEM_SPAWN, MAX_ITEM_SPAWN);
	for (int8 i = 0; i < spawnMax; i++)
	{
		int8 type = FMath::RandRange(0, ItemBoxAssets.Num() - 1);
		FVector spawnLocation = itemSpawnPoints[i];
		spawnLocation = spawnLocation + FVector(0, 0, 200);
		AItemBoxBase* item = GetWorld()->SpawnActor<AItemBoxBase>(ItemBoxAssets[type], spawnLocation, FRotator::ZeroRotator);
		item->InitItemBox(false);
		Target->AddItemBoxList(item);
	}
}

void AResourceManager::SpawnRewardBox(class AStageData* Target)
{
	TArray<FVector> rewardBoxSpawnPoint = Target->GetRewardBoxSpawnPoint();
	FActorSpawnParameters actorSpawnParameters;
	actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ARewardBoxBase* rewardBox;
	if (IsValid(RewardBoxAssets[0]))
	{
		if (IsValid(Target))
		{
			if (rewardBoxSpawnPoint.IsValidIndex(0))
			{
				rewardBox = GetWorld()->SpawnActor<ARewardBoxBase>(RewardBoxAssets[0], rewardBoxSpawnPoint[0], FRotator(0, 90, 0), actorSpawnParameters);
				if (IsValid(rewardBox)) Target->SetRewardBox(rewardBox);

			}
			
		}
		
	}
}

void AResourceManager::BindDelegate(class AStageData* Target)
{
	for (TWeakObjectPtr<class AEnemyCharacterBase> enemy : Target->GetMonsterList())
	{
		FString name = enemy->GetController()->GetName();
		Target->AIOffDelegate.AddDynamic(Cast<AAIControllerBase>(enemy->GetController()), &AAIControllerBase::DeactivateAI);
		Target->AIOnDelegate.AddDynamic(Cast<AAIControllerBase>(enemy->GetController()), &AAIControllerBase::ActivateAI);
		enemy->EnemyDeathDelegate.BindUFunction(this, FName("DieMonster"));
	}
}

void AResourceManager::DieMonster(AEnemyCharacterBase* Target)
{
	if (!IsValid(GetOwner())) return;
	TWeakObjectPtr<AStageData> currentStage = Cast<AChapterManagerBase>(GetOwner())->GetCurrentStage();

	UE_LOG(LogTemp, Log, TEXT("Call MonsterDie()"));

	currentStage->RemoveMonsterList(Target);

	if (currentStage->GetMonsterList().IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("AResourceManager::DieMonster: Stage Clear BroadCast StgaeClearDelegate"));

		currentStage->SetIsClear(true);
		SpawnRewardBox(currentStage.Get());

		Cast<AChapterManagerBase>(GetOwner())->CheckChapterClear();
	}
}

TSubclassOf<AEnemyCharacterBase> AResourceManager::GetEnemyWithID(int32 EnemyID)
{
	TSubclassOf<AEnemyCharacterBase> enemy;

	switch (EnemyID)
	{
	case 1001:
		enemy = EnemyAssets[0];
		break;
	case 1002:
		enemy = EnemyAssets[0];
		break;
	case 1003:
		enemy = EnemyAssets[2];
		break;
	case 1100:
		enemy = EnemyAssets[1];
		break;
	case 1101:
		enemy = EnemyAssets[3];
		break;
	case 1102:
		enemy = EnemyAssets[3];
		break;
	case 1200:
		enemy = EnemyAssets[4];
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("Wrong ID"));
		return nullptr;
		break;
	}
	return enemy;
}

FName AResourceManager::GetRandomMap()
{
	uint8 idx = FMath::RandRange(0, MapNames.Num() - 1);
	return MapNames[idx];
}


void AResourceManager::CleanAllResource()
{
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::CleanAllResource"));

	TArray<AStageData*> stages = Cast<AChapterManagerBase>(GetOwner())->GetStages();

	for (int32 idx = stages.Num()-1; idx>=0; idx--)
	{
		AStageData* stage = stages[idx];
		if (IsValid(stage) && !stage->IsActorBeingDestroyed())
		{
			CleanStage(stage);
		}
	}
	ABackStreetGameModeBase* gamemodeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(gamemodeRef))
	{
		gamemodeRef->GetGlobalDebuffManagerRef()->ClearAllDebuffTimer();
	}

	if(IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		Cast<AMainCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->ClearAllTimerHandle();
}

void AResourceManager::CleanStage(class AStageData* Target)
{
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::CleanStage"));

	CleanStageMonster(Target);
	CleanStageItem(Target);
	if (IsValid(Target->GetRewardBox()))
	{
		Target->GetRewardBox()->Destroy();
	}
	//if (Target->LevelRef != nullptr)
	//{
	//	// �����..
	//}
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AResourceManager::CleanStageMonster(class AStageData* Target)
{
	for (int32 idx = Target->GetMonsterList().Num() - 1; idx >= 0; idx--)
	{
		TWeakObjectPtr<class AEnemyCharacterBase> target = Target->GetMonsterIdx(idx);
		if (target.IsValid()&&!target->IsActorBeingDestroyed())
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(target.Get());
			target->TakeDamage(100000.0f, FDamageEvent(), nullptr, this);
		}
	}
}

void AResourceManager::CleanStageItem(class AStageData* Target)
{
	for (int32 idx = Target->GetItemBoxList().Num() - 1; idx >= 0; idx--)
	{
		TWeakObjectPtr<class AItemBoxBase> target = Target->GetItemBoxIdx(idx);
		if (target.IsValid() && !target->IsActorBeingDestroyed())
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(target.Get());
			target->Destroy();
		}
	}

	for (int32 idx = Target->GetItemList().Num() - 1; idx >= 0; idx--)
	{
		TWeakObjectPtr<class AItemBase> target = Target->GetItemIdx(idx);
		if (target.IsValid() && !target->IsActorBeingDestroyed())
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(target.Get());
			target->Destroy();
		}
	}
}


