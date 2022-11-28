#pragma once

#include "Engine/DataTable.h"
#include "CharacterInfoStructBase.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//PlayerMaxHP�� 1.0f
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.5f, UIMax = 10.0f))
		float CharacterMaxHP = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.1f, UIMax = 10.0f))
		float CharacterAtkMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 100.0f, UIMax = 1000.0f))
		float CharacterMoveSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = -1.0f, UIMax = 1.0f))
		float CharacterDefense = 0.0f;
};

USTRUCT(BlueprintType)
struct FCharacterStateStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//ĳ������ ����� ���� (Bit-Field�� ǥ��)
	UPROPERTY(BlueprintReadOnly)
		int32 CharacterDebuffState = (1<<10);

	//ĳ������ ���� ����
	UPROPERTY(BlueprintReadOnly)
		int32 CharacterBuffState = (1 << 10);

	//���� ������ �ִ���?
	UPROPERTY(BlueprintReadOnly)
		bool bIsRolling = false;

	//���� ������ �ϰ� �ִ���?
	UPROPERTY(BlueprintReadOnly)
		bool bIsAttacking = false;;

	//PlayerMaxHP�� 1.0f
	UPROPERTY(BlueprintReadOnly)
		float CharacterCurrHP;
};

UENUM(BlueprintType)
enum class ECharacterDebuffType : uint8
{
	E_Flame			UMETA(DisplayName = "Flame"),
	E_Poison		UMETA(DisplayName = "Poison"),
	E_Slow			UMETA(DisplayName = "Slow"),
	E_Sleep			UMETA(DisplayName = "Sleep"),
	E_DefenseDown	UMETA(DisplayName = "DefenseDown"),
	E_AttackDown	UMETA(DisplayName = "AttackDown")
};

UENUM(BlueprintType)
enum class ECharacterBuffType : uint8
{
	E_SpeedUp			UMETA(DisplayName = "SpeedUp"),
	E_Healing			UMETA(DisplayName = "Healing"),
	E_FastAtk			UMETA(DisplayName = "FastAtk"),
	E_DefenseUp			UMETA(DisplayName = "DefenseUp"),
	E_AttackUp			UMETA(DisplayName = "AttackUp"),
	E_Invincibility		UMETA(DisplayName = "Invincibility")
};