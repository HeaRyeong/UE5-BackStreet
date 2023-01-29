#pragma once

#include "Engine/DataTable.h"
#include "../../Character/public/CharacterInfoStructBase.h"
#include "WeaponStatStructBase.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
	E_None				UMETA(DisplayName = "None"),
	E_Hit				UMETA(DisplayName = "Hit"),
	E_Attack			UMETA(DisplayName = "Attack"),
	E_Explosion			UMETA(DisplayName = "Explosion")
};

USTRUCT(BlueprintType)
struct FProjectileStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//���� Ÿ���� �߻�ü����? (RadialDamage)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsExplosive;

	//�߻�ü�� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileSpeed = 2000.0f;

	//�߻�ü�� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileDamage = 0.2f;

	//�߷� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float GravityScale = 1.0f;

	//������ �Ǵ���?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsHoming = false;

	//�߻�ü�� �� �ϳ��� ������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;
};

USTRUCT(BlueprintType)
struct FWeaponStatStruct
{
public:
	GENERATED_USTRUCT_BODY()
	
	//----- ���� ���� Property --------------------

	//���� ���� ����
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponMeleeAtkRange = 160.0f;

	//���� ������ ������ ��? 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bCanMeleeAtk = true;

	//���� �ӵ� Rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate = 1.0f;

	//���� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamage = 0.2f;

	//����� �� �ϳ��� ������� ���� (�ӽ�)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;

	// ������ PROPERTY �߰�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteDurability = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 Durability = 10;

	//----- �߻�ü ���� Property ------------------
	//���� ź������?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsInfiniteAmmo;

	//���� ��, �߻�ü�� �ִ���?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bHasProjectile = false;

	//źâ�� �ʿ������? (ex. �⺻ ����ü ��)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteMagazine;

	//�� źâ�� �ִ� �߻�ü ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxAmmoPerMagazine; 

	//���� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float LoadingDelayTime;

	//�߻�ü ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FProjectileStatStruct ProjectileStat;

};

