// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ResourceManager.h"
#include "../public/StageData.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../public/ChapterManagerBase.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../AISystem/public/AIControllerBase.h"
#include "../public/RewardBoxBase.h"
#include "../../Item/public/ItemBoxBase.h"
#include "../../Item/public/ItemBase.h"
#include "Engine/LevelStreaming.h"

void AResourceManager::InitResourceManager()
{
	ABackStreetGameModeBase* gameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// AI Controller 반환 관련해서 논의 필요할 듯
	//gameModeRef->ClearResourceDelegate.AddDynamic(this, &AResourceManager::CleanAllResource);
	gameModeRef->ChapterClearResourceDelegate.AddDynamic(this, &AResourceManager::CleanAllResource);
}

void AResourceManager::SpawnStageActor(class AStageData* Target)
{
	UE_LOG(LogTemp, Log, TEXT("AResourceManager::SpawnStageActor -> Start Spawn"));
	if (Target->StageType == EStageCategoryInfo::E_Normal)
	{
		SpawnMonster(Target);
		SpawnItem(Target);
	}
	else if (Target->StageType == EStageCategoryInfo::E_Boss)
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

	for (int i = 0; i < 100; i++)
	{
		int32 selectidxA = FMath::RandRange(0, Target->MonsterSpawnPoints.Num() - 1);
		int32 selectidxB = FMath::RandRange(0, Target->MonsterSpawnPoints.Num() - 1);
		AActor* temp;

		temp = Target->MonsterSpawnPoints[selectidxA];
		Target->MonsterSpawnPoints[selectidxA] = Target->MonsterSpawnPoints[selectidxB];
		Target->MonsterSpawnPoints[selectidxB] = temp;

	}


	for (int32 i = 0; i < spawnNum; i++)
	{
		int32 enemyIDIdx = FMath::RandRange(0, enemyIDList.Num() - 1);

		FActorSpawnParameters actorSpawnParameters;
		FVector spawnLocation = Target->MonsterSpawnPoints[i]->GetActorLocation();
		spawnLocation = spawnLocation + FVector(0, 0, 200);

		actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyCharacterBase* monster = GetWorld()->SpawnActor<AEnemyCharacterBase>(GetEnemyWithID(enemyIDList[enemyIDIdx]),spawnLocation, FRotator::ZeroRotator, actorSpawnParameters);
		Target->MonsterList.Add(monster);
		monster->EnemyID = enemyIDList[enemyIDIdx];
		monster->InitEnemyStat();
	}

}

void AResourceManager::SpawnBossMonster(class AStageData* Target)
{
	FActorSpawnParameters actorSpawnParameters;
	uint16 idx = FMath::RandRange(0, Target->MonsterSpawnPoints.Num() - 1);
	FVector spawnLocation = Target->MonsterSpawnPoints[idx]->GetActorLocation();
	spawnLocation = spawnLocation + FVector(0, 0, 200);

	actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AEnemyCharacterBase* monster = GetWorld()->SpawnActor<AEnemyCharacterBase>(BossAssets[0], spawnLocation, FRotator::ZeroRotator, actorSpawnParameters);
	Target->MonsterList.Add(monster);
	// 수정 필요 하드코딩..
	monster->EnemyID = 1200;
	monster->InitEnemyStat();


}

void AResourceManager::SpawnItem(class AStageData* Target)
{
	for (int i = 0; i < 100; i++)
	{
		int32 selectidxA = FMath::RandRange(0, Target->ItemSpawnPoints.Num() - 1);
		int32 selectidxB = FMath::RandRange(0, Target->ItemSpawnPoints.Num() - 1);
		AActor* temp;

		temp = Target->ItemSpawnPoints[selectidxA];
		Target->ItemSpawnPoints[selectidxA] = Target->ItemSpawnPoints[selectidxB];
		Target->ItemSpawnPoints[selectidxB] = temp;

	}

	int8 spawnMax = FMath::RandRange(MIN_ITEM_SPAWN, MAX_ITEM_SPAWN);
	for (int8 i = 0; i < spawnMax; i++)
	{
		int8 type = FMath::RandRange(0, ItemBoxAssets.Num() - 1);
		FVector spawnLocation = Target->ItemSpawnPoints[i]->GetActorLocation();
		spawnLocation = spawnLocation + FVector(0, 0, 200);
		AItemBoxBase* item = GetWorld()->SpawnActor<AItemBoxBase>(ItemBoxAssets[type], spawnLocation, FRotator::ZeroRotator);
		item->InitItemBox(false);
		Target->ItemBoxList.AddUnique(item);
	}
}

void AResourceManager::SpawnRewardBox(class AStageData* Target)
{
	FActorSpawnParameters actorSpawnParameters;
	actorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ARewardBoxBase* rewardBox = GetWorld()->SpawnActor<ARewardBoxBase>(RewardBoxAssets[0], Target->RewardBoxSpawnPoint[0]->GetActorLocation(), FRotator(0, 90, 0), actorSpawnParameters);
	rewardBox->Tags.AddUnique("RewardBox");
	Target->RewardBoxRef=rewardBox;
	rewardBox->SetBelongStage(Target);
}

void AResourceManager::BindDelegate(class AStageData* Target)
{
	for (AEnemyCharacterBase* enemy : Target->MonsterList)
	{
		FString name = enemy->GetController()->GetName();
		Target->AIOffDelegate.AddDynamic(Cast<AAIControllerBase>(enemy->GetController()), &AAIControllerBase::DeactivateAI);
		Target->AIOnDelegate.AddDynamic(Cast<AAIControllerBase>(enemy->GetController()), &AAIControllerBase::ActivateAI);
		enemy->EnemyDeathDelegate.BindUFunction(this, FName("DieMonster"));
	}
}

void AResourceManager::DieMonster(AEnemyCharacterBase* Target)
{
	AStageData* currentStage = Cast<AChapterManagerBase>(GetOwner())->GetCurrentStage();

	UE_LOG(LogTemp, Log, TEXT("Call MonsterDie()"));

	currentStage->MonsterList.Remove(Target);

	if (currentStage->MonsterList.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("AResourceManager::DieMonster: Stage Clear BroadCast StgaeClearDelegate"));

		ABackStreetGameModeBase* gameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		currentStage->bIsClear = true;

		SpawnRewardBox(currentStage);
		gameModeRef->StageClearDelegate.Broadcast();

		Cast<AChapterManagerBase>(GetOwner())->CheckChapterClear();
		//PauseTimer();
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

	for (AStageData* stage : stages)
	{
		CleanStage(stage);
	}

}

void AResourceManager::CleanStage(class AStageData* Target)
{
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::CleanStage"));

	CleanStageMonster(Target);
	CleanStageItem(Target);
	if (Target->RewardBoxRef != nullptr)
	{
		Target->RewardBoxRef->Destroy();
	}
	if (Target->LevelRef != nullptr)
	{
		// 고민중..
	}
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Target->Destroy();

}


void AResourceManager::CleanStageMonster(class AStageData* Target)
{
	for (AEnemyCharacterBase* target :Target->MonsterList)
	{
		if (target != nullptr)
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(target);
			target->Destroy();
		}
	}
}


void AResourceManager::CleanStageItem(class AStageData* Target)
{
	for (AItemBoxBase* target : Target->ItemBoxList)
	{
		if (target != nullptr)
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(target);
			target->Destroy();
		}
	}

	for (AItemBase* target : Target->ItemList)
	{
		if (target != nullptr)
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(target);
			target->Destroy();
		}
	}
	// 임시 아이템 삭제 로직
	TArray<AActor*> itmes;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(), itmes);
	for (AActor* target : itmes)
	{
		UE_LOG(LogTemp, Log, TEXT("AChapterManagerBase::InitChapterManager: Find ResourceManager"));

		GetWorld()->GetTimerManager().ClearAllTimersForObject(target);
		target->Destroy();

	}
}


