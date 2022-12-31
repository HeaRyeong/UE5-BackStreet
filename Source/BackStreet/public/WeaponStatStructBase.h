#pragma once

#include "Engine/DataTable.h"
#include "CharacterInfoStructBase.h"
#include "WeaponStatStructBase.generated.h"

USTRUCT(BlueprintType)
struct FProjectileStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

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


	//----- �߻�ü ���� Property ------------------
	
	//���� ź������?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsInfiniteAmmo;

	//���� ��, �߻�ü�� �ִ���?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bHasProjectile;

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