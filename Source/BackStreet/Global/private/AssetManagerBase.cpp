
#include "../public/AssetManagerBase.h"
#include "../../StageSystem/public/TileBase.h"
#include "../../Item/public/ItemBase.h"
#include "Engine/AssetManager.h"
#include "../public/BackStreetGameModeBase.h"
#include "../../Item/public/ItemInfoStruct.h"
#include "../../Item/public/ItemDataAssetInfo.h"
#include "../../StageSystem/public/MissionBase.h"
#include "../../Character/public/EnemyCharacterBase.h"

UAssetManagerBase::UAssetManagerBase()
{

}


void UAssetManagerBase::LoadItemAsset(EItemCategoryInfo Type, TArray<AActor*> SpawnPoints)
{
	UE_LOG(LogTemp, Log, TEXT("Call LoadItemAsset"));

	switch (Type)
	{
	case EItemCategoryInfo::E_None:
		break;
	case EItemCategoryInfo::E_Weapon:
		break;
	case EItemCategoryInfo::E_Bullet:
		break;
	case EItemCategoryInfo::E_Buff:
		if (BuffAssets.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[LoadItemAsset] BuffAsstes num %d"), BuffAssets.Num());
			AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(BuffAssets, FStreamableDelegate::CreateUObject(this, &UAssetManagerBase::SpawnBuffItem,Type,SpawnPoints));
		}
		break;
	case EItemCategoryInfo::E_DeBuff:
		break;
	case EItemCategoryInfo::E_StatUp:
		break;
	case EItemCategoryInfo::E_Mission:
		if (MissionAssets.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("[LoadItemAsset] MissionAsstes num %d"), MissionAssets.Num());
			AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(MissionAssets, FStreamableDelegate::CreateUObject(this, &UAssetManagerBase::SpawnMissionItem, Type, SpawnPoints));
		}
		break;
	default:
		break;
	}
	
}

void UAssetManagerBase::SpawnBuffItem(EItemCategoryInfo Type, TArray<AActor*> SpawnPoints)
{
	int32 MaxSpawn = FMath::RandRange(1, 3);

	for (int32 i = 0; i < MaxSpawn; i++)
	{
		int32 BuffType = FMath::RandRange(0, 5);
		if (BuffAssets[BuffType].IsValid())
		{
			UBlueprint* Gen = Cast<UBlueprint>(BuffAssets[BuffType].ResolveObject());
			AItemBase* Target = GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
			// AItemBase 클래스 스폰하고 Init 하기
			Target->InitItem(Type);

		}
	}
}

void UAssetManagerBase::SpawnMissionItem(EItemCategoryInfo Type, TArray<AActor*> SpawnPoints)
{
	int32 MaxSpawn = 1;

	for (int32 i = 0; i < MaxSpawn; i++)
	{
		//int32 BuffType = FMath::RandRange(0, 5);
		if (MissionAssets[0].IsValid())
		{
			UBlueprint* Gen = Cast<UBlueprint>(MissionAssets[0].ResolveObject());
			AItemBase* Target = GetWorld()->SpawnActor<AItemBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
			// AItemBase 클래스 스폰하고 Init 하기
			Target->InitItem(Type);

		}
	}
}

void UAssetManagerBase::LoadMonsterAsset(TArray<AActor*> SpawnPoints, class ATileBase* TileRef)
{
	AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(BuffAssets, FStreamableDelegate::CreateUObject(this, &UAssetManagerBase::SpawnMonster, SpawnPoints, TileRef));
}

void UAssetManagerBase::SpawnMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef)
{
	int32 MaxSpawn = FMath::RandRange(1, 3);

	for (int32 i = 0; i < MaxSpawn; i++)
	{
		if (MonsterAssets[0].IsValid())
		{
			UBlueprint* Gen = Cast<UBlueprint>(MonsterAssets[0].ResolveObject());
			AEnemyCharacterBase* Target = GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
			TileRef->MonsterList.AddUnique(Target);
			
		}
	}

	TileRef->BindDelegate();
}

void UAssetManagerBase::LoadMissionMonsterAsset(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, class UMissionBase* MissionRef)
{
	AssetStreamingHandle = GameModeRef->StreamableManager.RequestAsyncLoad(BuffAssets, FStreamableDelegate::CreateUObject(this, &UAssetManagerBase::SpawnMissionMonster, SpawnPoints, TileRef,MissionRef));
}

void UAssetManagerBase::SpawnMissionMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, class UMissionBase* MissionRef)
{
	int32 MaxSpawn = FMath::RandRange(1, 3);

	for (int32 i = 0; i < MaxSpawn; i++)
	{
		if (MonsterAssets[0].IsValid())
		{
			UBlueprint* Gen = Cast<UBlueprint>(MonsterAssets[0].ResolveObject());
			AEnemyCharacterBase* Target = GetWorld()->SpawnActor<AEnemyCharacterBase>(Gen->GeneratedClass, SpawnPoints[i]->GetActorLocation(), FRotator::ZeroRotator);
			Target->Tags.AddUnique("MissionMonster");
			TileRef->MonsterList.AddUnique(Target);
			MissionRef->MonsterList.AddUnique(Target);

		}
	}

	TileRef->BindDelegate();
}