#pragma once

#include "Engine/DataTable.h"
#include "WeaponStatStructBase.generated.h"

USTRUCT(BlueprintType)
struct FProjectileStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileDamage = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float GravityScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsHoming = false;
};
