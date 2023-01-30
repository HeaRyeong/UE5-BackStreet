#pragma once

#include "Engine/DataTable.h"
#include "../../Character/public/CharacteInfoEnum.h"
#include "ItemInfoStruct.generated.h"


UENUM(BlueprintType)
enum class EItemCategoryInfo : uint8
{
	E_None					UMETA(DisplayName = "None"),
	E_Weapon				UMETA(DisplayName = "Weapon"),
	E_Bullet				UMETA(DisplayName = "Bullet"),
	E_Buff					UMETA(DisplayName = "Buff"),
	E_DeBuff				UMETA(DisplayName = "DeBuff"),
	E_StatUp				UMETA(DisplayName = "StatUp"),
	E_Mission				UMETA(DisplayName = "Mission"),
};

UENUM(BlueprintType)
enum class EStatUpCategoryInfo : uint8
{
	E_None					UMETA(DisplayName = "None"),
	E_MaxHp					UMETA(DisplayName = "MaxHp"),
	E_ATK					UMETA(DisplayName = "ATKUp"),
	E_ATKSpeed				UMETA(DisplayName = "ATKSpeed"),
	E_MoveSpeed				UMETA(DisplayName = "MoveSpeed"),
	E_Defense				UMETA(DisplayName = "Defense"),

};


USTRUCT(BlueprintType)
struct FBuffItemInfoStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		ECharacterBuffType Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Value;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Time;

};

USTRUCT(BlueprintType)
struct FDeBuffItemInfoStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		ECharacterDebuffType Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Value;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Time;

};

USTRUCT(BlueprintType)
struct FWeaponItemInfoStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteDurability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 Durability = 10;

	// --------- ���Ÿ� ���� -------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteMagazine;
	
	//�� źâ�� �ִ� �߻�ü ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxAmmoPerMagazine;
		
};

USTRUCT(BlueprintType)
struct FProjectileItemInfoStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsExplosive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float GravityScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsHoming;

	//�߻�ü�� �� �ϳ��� ������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;

};