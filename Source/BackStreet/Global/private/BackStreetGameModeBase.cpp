// Copyright Epic Games, Inc. All Rights Reserved.

#include "../public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/GridBase.h"
#include "../../StageSystem/public/TileBase.h"
#include "../../Item/public/ProjectileBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Character/public/CharacterBase.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ABackStreetGameModeBase::ABackStreetGameModeBase()
{
	AssetDataManager = CreateDefaultSubobject<UAssetManagerBase>(TEXT("AssetManager"));

}

void ABackStreetGameModeBase::InitGame()
{
	AssetDataManager->GameModeRef = this;
	RemainChapter = 2;
	ChapterStatValue = 0;
	InitChapter();
}

void ABackStreetGameModeBase::InitChapter()
{
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;

	Chapter = GetWorld()->SpawnActor<AGridBase>(AGridBase::StaticClass(), spawnLocation, rotator, spawnParams);
	Chapter->CreateMaze(3, 3);

	CurrTile = Chapter->GetCurrentTile();
	RemainChapter--;
}

void ABackStreetGameModeBase::NextStage(uint8 Dir)
{
	switch ((EDirection)Dir)
	{
	case EDirection::E_UP:
		PreDir = (uint8)(EDirection::E_DOWN);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : DOWN"));
		break;
	case EDirection::E_DOWN:
		PreDir = (uint8)(EDirection::E_UP);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : UP"));
		break;
	case EDirection::E_LEFT:
		PreDir = (uint8)(EDirection::E_RIGHT);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : RIGHT"));
		break;
	case EDirection::E_RIGHT:
		PreDir = (uint8)(EDirection::E_LEFT);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : LEFT"));
		break;
	default:
		break;
	}
	MoveTile(Dir);
}

void ABackStreetGameModeBase::MoveTile(uint8 NextDir)
{
	Chapter->MoveCurrentTile(NextDir);
	CurrTile = Chapter->GetCurrentTile();
}

void ABackStreetGameModeBase::ClearChapter()
{
	if (IsValid(Chapter))
		Chapter->RemoveChapter();

	if (RemainChapter == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("GameClear"));

	}
	else
	{
		InitChapter();
		ChapterStatValue += 0.1f;
		CurrTile->LoadLevel();
	}
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

void ABackStreetGameModeBase::UpdateCharacterStatWithID(ACharacterBase* TargetCharacter, const uint8 CharacterID)
{
	if (IsValid(TargetCharacter) && TargetCharacter->ActorHasTag("Enemy"))
	{
		//DataTable로 부터 Read
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
	if (IsValid(TargetWeapon))
	{
		FString rowName = FString::FromInt(WeaponID);
		FWeaponStatStruct* newStat = WeaponStatTable->FindRow<FWeaponStatStruct>(FName(rowName), rowName);
		if (newStat != nullptr)
		{
			TargetWeapon->UpdateWeaponStat(*newStat);
		}
	}
}

void ABackStreetGameModeBase::UpdateProjectileStatWithID(AProjectileBase* TargetProjectile, const uint8 ProjectileID)
{
	if (IsValid(TargetProjectile))
	{
		FString rowName = FString::FromInt(ProjectileID);
		FProjectileStatStruct* newStat = ProjectileStatTable->FindRow<FProjectileStatStruct>(FName(rowName), rowName);
		if (newStat != nullptr)
		{
			TargetProjectile->UpdateProjectileStat(*newStat);
		}
	}
}