// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "ItemInfoStruct.h"
#include "WeaponInfoStruct.h"
#include "ProjectileInfoStruct.h"
#include "ItemDataAssetInfo.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API UItemDataAssetInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		EItemCategoryInfo Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		class UNiagaraSystem* NiaSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		class UNiagaraSystem* NiaSystemEnd;


	// Weapon 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UStaticMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FWeaponItemInfoStruct WeaponStat;


	// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		UStaticMesh* ProjectiletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		FProjectileItemInfoStruct ProjectileStat;

	// Buff

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
		FBuffItemInfoStruct BuffStat;

	// DeBuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeBuff")
		FDeBuffItemInfoStruct DeBuffStat;
};

