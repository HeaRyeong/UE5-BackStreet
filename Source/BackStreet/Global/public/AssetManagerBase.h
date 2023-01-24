#pragma once
#include "BackStreet.h"
#include "Engine/StreamableManager.h"
#include "../../Item/public/ItemInfoStruct.h"
#include "Engine/StreamableManager.h"
#include "AssetManagerBase.generated.h"

UCLASS(Config = Game, defaultconfig)
class BACKSTREET_API UAssetManagerBase : public UObject
{
	GENERATED_BODY()

public:
	UAssetManagerBase();

public:
	UFUNCTION()
		void LoadItemAsset(EItemCategoryInfo Type, TArray<AActor*> SpawnPoints);
	UFUNCTION()
		void SpawnBuffItem(EItemCategoryInfo Type, TArray<AActor*> SpawnPoints);
	UFUNCTION()
		void SpawnMissionItem(EItemCategoryInfo Type, TArray<AActor*> SpawnPoints);

	UFUNCTION()
		void LoadMonsterAsset(TArray<AActor*> SpawnPoints,class ATileBase* TileRef);
	UFUNCTION()
		void SpawnMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef);

	UFUNCTION()
		void LoadMissionMonsterAsset(TArray<AActor*> SpawnPoints, class ATileBase* TileRe, class UMissionBase* MissionReff);
	UFUNCTION()
		void SpawnMissionMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, class UMissionBase* MissionRef);

public:
	UPROPERTY(Config)
		TArray<FSoftObjectPath> MonsterAssets;
	UPROPERTY(Config)
		TArray<FSoftObjectPath> BuffAssets;
	UPROPERTY(Config)
		TArray<FSoftObjectPath> MissionAssets;

		TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

		UPROPERTY()
			class ABackStreetGameModeBase* GameModeRef;

};
