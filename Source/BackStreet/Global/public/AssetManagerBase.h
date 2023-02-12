#pragma once
#include "BackStreet.h"
#include "Engine/StreamableManager.h"
#include "AssetManagerBase.generated.h"

#define StageTypeNum 5

UCLASS()
class BACKSTREET_API AAssetManagerBase : public AActor
{
	GENERATED_BODY()

public:
	AAssetManagerBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		TSubclassOf<AEnemyCharacterBase> GetEnemyWithID(int32 EnemyID);

	UFUNCTION()
		FName GetRandomMap();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AEnemyCharacterBase>> EnemyAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AItemBase>> ItemAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AWeaponBase>> WeaponAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AProjectileBase>> ProjectileAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<class AItemBase>> MissionAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> MapNames;

private:
	UPROPERTY()
		class ABackStreetGameModeBase* GameModeRef;

};

