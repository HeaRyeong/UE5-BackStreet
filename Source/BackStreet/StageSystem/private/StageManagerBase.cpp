// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/StageManagerBase.h"
#include "../public/GridBase.h"
#include "../public/TileBase.h"
#include "../public/ChapterManagerBase.h"
#include "Engine/LevelStreaming.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "../../Item/public/ItemBoxBase.h"
#include "../../Item/public/ItemBase.h"
#include "../public/ALevelScriptInGame.h"
#include "../public/LevelScriptBase.h"
#include "../../Item/public/WeaponBase.h"
#include "Engine/LevelStreaming.h"



// Sets default values
AStageManagerBase::AStageManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageManagerBase::InitStageManager()
{
	CharacterRef = Cast<AMainCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));	

}

void AStageManagerBase::SetStage(AGridBase* Chapter)
{
	if (Stages.IsEmpty())
	{
		for (ATileBase* tile : Chapter->StageArray)
		{
			Stages.Add(tile);
		
			tile->StageClearDelegate.AddDynamic(this, &AStageManagerBase::ClearStageReward);
		}
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Check Stages :: Is Not Empty"));
	
	CurrentTile = nullptr;
	SetMissionStages();
}

void AStageManagerBase::CleanManager()
{

	for (ATileBase* target : Stages)
	{
		TArray<AEnemyCharacterBase*> monsterList = target->GetMonsterList();
		TArray<AItemBoxBase*> itemBoxList = target->GetItemBoxList();

		for (int32 i = 0; i < monsterList.Num(); i++)
		{
			if (!monsterList.IsValidIndex(i))
			{
				monsterList.RemoveAt(i);
				i--;
			}
		}
		for (AEnemyCharacterBase* remove : target->GetMonsterList())
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(remove);
			remove->Die();
		}

		for (int32 i = 0; i < itemBoxList.Num(); i++)
		{
			if (!itemBoxList.IsValidIndex(i))
			{
				itemBoxList.RemoveAt(i);
				i--;
			}
		}
		for (AItemBoxBase* remove : target->GetItemBoxList())
		{
			if(remove!=nullptr)
				remove->Destroy();
		}

		target->ClearTimer();
	}

	TArray<AActor*> items;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(),items);
	
	for (AActor* remove : items)
	{
		if (remove != nullptr)
			remove->Destroy();
	}
	Stages.Empty();
	CurrentTile = nullptr;
}	
	
void AStageManagerBase::SetMissionStages()
{	
	
}	
	
void AStageManagerBase::MoveStage(uint8 Dir)
{	
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase:: MoveStage Dir: %d "),Dir);
	if (CurrentTile != nullptr)
	{
		UnloadTile = CurrentTile;
		UnloadLevel = CurrentTile->LevelRef;
		UnloadTile->PauseStage();
	}
	MoveDir = (EDirection)Dir;

	switch (MoveDir)
	{
	case EDirection::E_UP:
		CurrentTile = GetStage(CurrentTile->XPos + 1, CurrentTile->YPos);
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Up"));
		break;
	case EDirection::E_DOWN:
		CurrentTile = GetStage(CurrentTile->XPos - 1, CurrentTile->YPos);
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Down"));
		break;
	case EDirection::E_LEFT:
		CurrentTile = GetStage(CurrentTile->XPos, CurrentTile->YPos - 1);
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Left"));
		break;
	case EDirection::E_RIGHT:
		CurrentTile = GetStage(CurrentTile->XPos, CurrentTile->YPos + 1);
		LoadStage();
		UE_LOG(LogTemp, Log, TEXT("Move to Right"));
		break;
	case EDirection::E_Start:
		UE_LOG(LogTemp, Log, TEXT("Start Game"));
		CurrentTile = Stages[0];
		InGameScriptRef->GetChapterManager()->GetChapter()->UnPauseTimer();
		LoadStage();
		break;
	case EDirection::E_Chapter:
		UE_LOG(LogTemp, Log, TEXT("New Chapter"));
		InGameScriptRef->GetChapterManager()->ClearChapter();
		//UnLoadStage();
		break;
	default:
		UE_LOG(LogTemp, Log, TEXT("Wrong Dir"));
		break;
	}
	// UI 업데이트
	InGameScriptRef->UpdateMiniMapUI();

}

void AStageManagerBase::LoadStage()
{
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::LoadStage"));
	//FScriptDelegate MyScriptDelegate;

	if (CurrentTile->LevelRef != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Load Level"));
		CurrentTile->LevelRef->SetShouldBeLoaded(true);
		CurrentTile->LevelRef->SetShouldBeVisible(true);
		//MyScriptDelegate.BindUFunction(this, "CompleteLoad");
		//CurrentTile->LevelRef->OnLevelLoaded.Add(MyScriptDelegate);
	
	}
	else
	{	
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , Create Level Instance"));
		FString name = FString::FromInt(InGameScriptRef->ChapterManager->GetChapterLV());
		name += FString(TEXT("Stage"));
		name += FString::FromInt(CurrentTile->YPos * MAX_GRID_SIZE + CurrentTile->XPos);
		CurrentTile->LevelRef = UGameplayStatics::GetStreamingLevel(GetWorld(), CurrentTile->LevelToLoad)->CreateInstance(name);
		CurrentTile->LevelRef->LevelTransform.SetLocation(CurrentTile->GetActorLocation());
		//MyScriptDelegate.BindUFunction(this, "CompleteLoad");
		//CurrentTile->LevelRef->OnLevelLoaded.Add(MyScriptDelegate);
		CurrentTile->LevelRef->SetShouldBeLoaded(true);
		CurrentTile->LevelRef->SetShouldBeVisible(true);	
	}
}

void AStageManagerBase::UnLoadStage()
{
	UE_LOG(LogTemp, Log, TEXT("AStageManagerBase::UnLoadStage"));



	if (UnloadLevel != nullptr) // 레벨 스트리밍 인스턴스 존재
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is exist, Now UnLoad Level"));
		UnloadLevel->SetShouldBeLoaded(false);
		UnloadLevel->SetShouldBeVisible(false);
		//UnloadDelegate.BindUFunction(this, "CompleteUnLoad");
		//CurrentTile->LevelRef->OnLevelLoaded.Add(UnloadDelegate);

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Instance is not exist , error"));

	}



}


void AStageManagerBase::ClearStageReward()
{

	//if (!IsValid(PlayerCharacterRef)) return;

	//FCharacterStatStruct NewStat = PlayerCharacterRef->GetCharacterStat();
	//float RewardValue;

	//if (CurrentTile->ClearTime < 1.0f) // A등급
	//{
	//	RewardValue = ChapterManager->GetChapterWeight() + 0.3f;
	//	UE_LOG(LogTemp, Log, TEXT("A Rank %f"), RewardValue);
	//}
	//else if (CurrentTile->ClearTime < 3.0f) // B등급
	//{
	//	RewardValue = ChapterManager->GetChapterWeight() + 0.2f;
	//	UE_LOG(LogTemp, Log, TEXT("B Rank %f"), RewardValue);
	//}
	//else // C등급
	//{
	//	RewardValue = ChapterManager->GetChapterWeight() + 0.1f;
	//	UE_LOG(LogTemp, Log, TEXT("C Rank %f"), RewardValue);
	//}

	//switch (RewardType)
	//{
	//case EStatUpCategoryInfo::E_None:
	//	break;
	//case EStatUpCategoryInfo::E_MaxHp:
	//	UE_LOG(LogTemp, Log, TEXT("MaxHp"));
	//	NewStat.CharacterMaxHP += 0.1f + RewardValue;
	//	break;
	//case EStatUpCategoryInfo::E_ATK:
	//	UE_LOG(LogTemp, Log, TEXT("ATK"));
	//	NewStat.CharacterAtkMultiplier += 0.1f + RewardValue;
	//	break;
	//case EStatUpCategoryInfo::E_ATKSpeed:
	//	UE_LOG(LogTemp, Log, TEXT("ATKSpeed"));
	//	NewStat.CharacterAtkSpeed += 0.05f + (RewardValue * 0.1);
	//	break;
	//case EStatUpCategoryInfo::E_MoveSpeed:
	//	UE_LOG(LogTemp, Log, TEXT("MoveSpeed"));
	//	NewStat.CharacterMoveSpeed += 20.0f + (RewardValue * 10);
	//	break;
	//case EStatUpCategoryInfo::E_Defense:
	//	UE_LOG(LogTemp, Log, TEXT("Defense"));
	//	NewStat.CharacterDefense += 0.1f + RewardValue;
	//	break;
	//default:
	//	break;
	//}
	//UpdateCharacterStat(PlayerCharacterRef, NewStat);

	UE_LOG(LogTemp, Log, TEXT("Call RewardStageClear"));
}



void AStageManagerBase::CompleteLoad()
{
	UE_LOG(LogTemp, Log, TEXT("Delegate Call CompleteLoad"));
}

void AStageManagerBase::CompleteUnLoad()
{/*
	UE_LOG(LogTemp, Log, TEXT("Delegate Call CompleteUnLoad"));
	ALevelScriptBase* script = Cast<ALevelScriptBase>(CurrentTile->LevelRef->GetLoadedLevel()->GetLevelScriptActor());
	
	script->InitLevel(CurrentTile);
	script->SetGate();
	script->TeleportCharacter();*/
}

ATileBase* AStageManagerBase::GetStage(int32 XPosition, int32 YPosition)
{
	int32 size = MAX_GRID_SIZE;

	if (XPosition >= 0 && XPosition < size && YPosition >= 0 && YPosition < size)
	{
		return (Stages[(YPosition * size) + XPosition]);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Out of Range X:%d Y:%d"), XPosition,YPosition);
		return nullptr;
	}
}
