#pragma once

#include "Engine/DataTable.h"
#include "../../Character/public/CharacterInfoEnum.h"
#include "ProjectileInfoStruct.h"
#include "WeaponInfoStruct.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Hit				UMETA(DisplayName = "Hit"),
	E_Attack			UMETA(DisplayName = "Attack"),
	E_Explosion			UMETA(DisplayName = "Explosion"),
	E_Boss				UMETA(DisplayName = "Boss")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Melee				UMETA(DisplayName = "Melee"),
	E_Throw				UMETA(DisplayName = "Throw"),
	E_Shoot				UMETA(DisplayName = "Shoot")
};

USTRUCT(BlueprintType)
struct FMeleeWeaponStatStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	//----- ���� ���� Property --------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponMeleeDamage = 0.2f;

	//���� ������ ������ ��? 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bCanMeleeAtk = true;

	//����� �� �ϳ��� ������� ���� (�ӽ�)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float DebuffTotalTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float DebuffVariable;
};

USTRUCT(BlueprintType)
struct FRangedWeaponStatStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()
	
	//----- �߻�ü ���� Property ------------------
	//���� ź������?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsInfiniteAmmo;

	//���� ��, �߻�ü�� �ִ���?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bHasProjectile = false;

	//���� źâ����?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteMagazine;

	//�� źâ�� �ִ� �߻�ü ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxAmmoPerMagazine;

	//������ �� �ִ� �ִ� �߻�ü ���� 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxTotalAmmo = 200;

	//���� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float LoadingDelayTime;
};

USTRUCT(BlueprintType)
struct FWeaponStatStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

		UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FName WeaponName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		FName Description;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		EWeaponType WeaponType;

	//----- ���� Stat -------
	//���� �ӵ� Rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate = 1.0f;

	//���� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamageRate = 1.0f;

	// ������ PROPERTY �߰�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfinite = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxDurability = 10;

	//�κ��丮�� �����ϴ� ĭ ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		uint8 WeaponWeight = 1;

	//----- �ٰŸ� Stat ------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FMeleeWeaponStatStruct MeleeWeaponStat;

	//----- ���Ÿ� Stat ------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FRangedWeaponStatStruct RangedWeaponStat;
};

USTRUCT(BlueprintType)
struct FRangedWeaponStateStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//���� źâ�� �ִ� �߻�ü ��
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentAmmoCount = 0;

	//źâ�� �ִ� źȯ���� ����
	UPROPERTY(BlueprintReadOnly)
		int32 TotalAmmoCount = 0;
};

USTRUCT(BlueprintType)
struct FWeaponStateStruct
{
public:						
	GENERATED_USTRUCT_BODY()

	//���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 CurrentDurability = 10;

	//�޺� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 ComboCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRangedWeaponStateStruct RangedWeaponState;
};