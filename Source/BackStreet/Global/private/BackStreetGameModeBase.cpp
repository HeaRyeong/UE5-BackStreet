// Copyright Epic Games, Inc. All Rights Reserved.

#include "../public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/GridBase.h"
#include "../../StageSystem/public/TileBase.h"
#include "../../StageSystem/public/ChapterManagerBase.h"
#include "../../StageSystem/public/StageManagerBase.h"
#include "../../Item/public/ProjectileBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "../public/AssetManagerBase.h"

ABackStreetGameModeBase::ABackStreetGameModeBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Character/EnemyCharacter/Data/D_EnemyStatDataTable.D_EnemyStatDataTable"));
	if (DataTable.Succeeded())
	{
		EnemyStatTable = DataTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> StageTypeDataTable(TEXT("/Game/Map/Stages/Data/D_StageEnemyTypeTable.D_StageEnemyTypeTable"));
	if (StageTypeDataTable.Succeeded())
	{
		StageTypeTable = StageTypeDataTable.Object;
	}

}

void ABackStreetGameModeBase::BeginPlay()
{
	//------ 델리게이트 바인딩 ---------------
	FinishChapterDelegate.AddDynamic(this, &ABackStreetGameModeBase::FinishChapter);

	//------ Ref 멤버 초기화  ---------------
	PlayerCharacterRef = Cast<AMainCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}


void ABackStreetGameModeBase::StartChapter()
{
	//FTimerHandle delegateBindDelayTimer;
	//GetWorldTimerManager().SetTimer(delegateBindDelayTimer, FTimerDelegate::CreateLambda([&]()
	//	{
	//		StartChapterDelegate.Broadcast(); //Binding이 되도록 한 Tick 이후에 BroadCast를 해준다.
	//	}), 0.1f, false, 0.5f);

}

void ABackStreetGameModeBase::RewardStageClear(EStatUpCategoryInfo RewardType)
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
}

void ABackStreetGameModeBase::PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius)
{
	if (CameraShakeEffectList.Num() < (uint8)EffectType) return;

	Location = Location + FVector(-700.0f, 0.0f, 1212.0f); //캐릭터의 Camera의 위치에 맞게 변환
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeEffectList[(uint8)EffectType], Location, Radius * 0.75f, Radius);
}

void ABackStreetGameModeBase::UpdateCharacterStat(ACharacterBase* TargetCharacter, FCharacterStatStruct NewStat)
{
	if (IsValid(TargetCharacter))
	{
		TargetCharacter->UpdateCharacterStat(NewStat);
	}
}

void ABackStreetGameModeBase::UpdateCharacterStatWithID(ACharacterBase* TargetCharacter, const uint32 CharacterID)
{
	if (IsValid(TargetCharacter) && TargetCharacter->ActorHasTag("Enemy"))
	{
		//DataTable로 부터 Read
		FString rowName = FString::FromInt(CharacterID);
		FEnemyStatStruct* typeInfo = EnemyStatTable->FindRow<FEnemyStatStruct>(FName(rowName), rowName);

		if (typeInfo)
		{
			FCharacterStatStruct NewStat;
			NewStat.CharacterMaxHP = typeInfo->CharacterMaxHP;
			NewStat.CharacterDefense = typeInfo->CharacterDefense;
			NewStat.CharacterAtkSpeed = typeInfo->CharacterAtkSpeed;
			NewStat.CharacterAtkMultiplier = typeInfo->CharacterAtkMultiplier;
			NewStat.CharacterMoveSpeed = typeInfo->CharacterMoveSpeed;

			TargetCharacter->UpdateCharacterStat(NewStat);
		}
	}
}

void ABackStreetGameModeBase::UpdateWeaponStat(AWeaponBase* TargetWeapon, FWeaponStatStruct NewStat)
{
	if (IsValid(TargetWeapon))
	{
		TargetWeapon->UpdateWeaponStat(NewStat);
	}
}

void ABackStreetGameModeBase::UpdateWeaponStatWithID(AWeaponBase* TargetWeapon, const uint8 WeaponID)
{
	if (IsValid(TargetWeapon) && IsValid(WeaponStatTable))
	{
		FString rowName = FString::FromInt(WeaponID);
		FWeaponStatStruct newStat = GetWeaponStatInfoWithID(WeaponID);
		TargetWeapon->UpdateWeaponStat(newStat);
	}
}

void ABackStreetGameModeBase::UpdateProjectileStatWithID(AProjectileBase* TargetProjectile, const uint8 ProjectileID)
{
	if (IsValid(TargetProjectile) && IsValid(ProjectileStatTable))
	{
		FString rowName = FString::FromInt(ProjectileID);
		FProjectileStatStruct* newStat = ProjectileStatTable->FindRow<FProjectileStatStruct>(FName(rowName), rowName);
		if (newStat != nullptr)
		{
			TargetProjectile->UpdateProjectileStat(*newStat);
		}
	}
}

FWeaponStatStruct ABackStreetGameModeBase::GetWeaponStatInfoWithID(const uint8 WeaponID)
{
	FString rowName = FString::FromInt(WeaponID);
	FWeaponStatStruct* newStat = WeaponStatTable->FindRow<FWeaponStatStruct>(FName(rowName), rowName);
	if (newStat == nullptr) return FWeaponStatStruct();
	return *newStat;
}

FStageEnemyTypeStruct ABackStreetGameModeBase::GetStageTypeInfoWithRow(uint16 row)
{
	FString rowName = FString::FromInt(row);
	FStageEnemyTypeStruct* newStat = StageTypeTable->FindRow<FStageEnemyTypeStruct>(FName(rowName), rowName);
	if (newStat == nullptr) return FStageEnemyTypeStruct();
	return *newStat;
}

