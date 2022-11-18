#pragma once

#include "Engine/DataTable.h"
#include "CharacterInfoStructBase.h"
#include "WeaponStatStructBase.generated.h"

USTRUCT(BlueprintType)
struct FProjectileStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//발사체의 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileSpeed = 2000.0f;

	//발사체의 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileDamage = 0.2f;

	//중력 스케일
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float GravityScale = 1.0f;

	//유도가 되는지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsHoming = false;

	//발사체는 각 하나의 디버프만 가짐
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;
};

USTRUCT(BlueprintType)
struct FMeleeWeaponStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamage = 0.2f;

	//무기는 각 하나의 디버프만 가짐 (임시)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;
};