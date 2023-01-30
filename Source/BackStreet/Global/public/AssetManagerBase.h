#pragma once
#include "BackStreet.h"
#include "Engine/StreamableManager.h"
#include "AssetManagerBase.generated.h"

#define NormalStageTypeNum 5

UCLASS()
class BACKSTREET_API AAssetManagerBase : public AActor
{
	GENERATED_BODY()

public:
	AAssetManagerBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void LoadWeaponAssets();
	UFUNCTION(BlueprintCallable)
		class AWeaponBase* SpawnWeaponwithID(int32 ID);
	/*UFUNCTION()
		class AProjectileBase* SpawnProjectilewithPath(FSoftObjectPath path, FTransform SpawnTransform, FActorSpawnParameters SpawnParams);*/
	UFUNCTION()
		void CheckWeaponLoad();


	UFUNCTION()
		void LoadMonsterAsset(TArray<AActor*> SpawnPoints,class ATileBase* TileRef);
	UFUNCTION()
		void LoadItemAsset(EItemCategoryInfo Type, AActor* SpawnPoints);
	UFUNCTION()
		void LoadMissionMonsterAsset(TArray<AActor*> SpawnPoints, class ATileBase* TileRe, class UMissionBase* MissionReff);
	
	UFUNCTION()
		void SpawnBuffItem(EItemCategoryInfo Type, AActor* SpawnPoints);
	UFUNCTION()
		void SpawnWeaponItem(EItemCategoryInfo Type, AActor* SpawnPoints);
	UFUNCTION()
		void SpawnProjectileItem(EItemCategoryInfo Type, AActor* SpawnPoints);
	UFUNCTION()
		void SpawnMissionItem(EItemCategoryInfo Type, AActor* SpawnPoints);
	UFUNCTION()
		void SpawnMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, TArray<int32> EnemyIDList, int8 SpawnNum);
	UFUNCTION()
		void SpawnMissionMonster(TArray<AActor*> SpawnPoints, class ATileBase* TileRef, class UMissionBase* MissionRef);

	UFUNCTION()
		FStageEnemyTypeStruct ReturnStageEnemyTypeStruct(int32 StageType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> EnemyAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> WeaponAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> WeaponItemAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> ProjectileItemAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> BuffAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> MissionAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> MonsterMaterial;

		TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
		TSharedPtr<struct FStreamableHandle> WeaponStreamingHandle;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
		class ABackStreetGameModeBase* GameModeRef;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Data")
		UDataTable* StageTypeTable;

};
