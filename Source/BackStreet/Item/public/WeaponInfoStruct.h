#pragma once

#include "Engine/DataTable.h"
#include "../../Character/public/CharacteInfoEnum.h"
#include "ProjectileInfoStruct.h"
#include "WeaponInfoStruct.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Hit				UMETA(DisplayName = "Hit"),
	E_Attack			UMETA(DisplayName = "Attack"),
	E_Explosion			UMETA(DisplayName = "Explosion")
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

	//----- ���� Stat -------
	
	//���� �ӵ� Rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate = 1.0f;

	//���� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamageRate = 1.0f;

	//�κ��丮�� �����ϴ� ĭ ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		uint8 WeaponWeight = 1;

	//----- ���� ���� Property --------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponMeleeDamage = 0.2f;

	//���� ������ ������ ��? 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bCanMeleeAtk = true;

	//����� �� �ϳ��� ������� ���� (�ӽ�)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;

	// ������ PROPERTY �߰�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteDurability = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxDurability = 10;

	//----- �߻�ü ���� Property ------------------
	//���� ź������?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsInfiniteAmmo;

	//���� ��, �߻�ü�� �ִ���?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bHasProjectile = false;

	// ���� źâ����?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteMagazine;

	//�� źâ�� �ִ� �߻�ü ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxAmmoPerMagazine; 

	//���� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float LoadingDelayTime;
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

	//���� źâ�� �ִ� �߻�ü ��
	UPROPERTY(BlueprintReadOnly)
		int32 CurrentAmmoCount = 1;

	//źâ�� �ִ� źȯ���� ����
	UPROPERTY(BlueprintReadOnly)
		int32 TotalAmmoCount = 0;
};