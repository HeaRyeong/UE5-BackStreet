#pragma once

#include "Engine/DataTable.h"
#include "CharacterInfoStructBase.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//PlayerMaxHP는 1.0f
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.5f, UIMax = 10.0f))
		float CharacterMaxHP = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.1f, UIMax = 10.0f))
		float CharacterAtkMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 100.0f, UIMax = 1000.0f))
		float CharacterMoveSpeed = 400.0f;
};

USTRUCT(BlueprintType)
struct FCharacterStateStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//캐릭터의 디버프 상태 (Bit-Field로 표현)
	UPROPERTY(BlueprintReadOnly)
		int32 CharacterDebuffState = (1<<3);

	//현재 구르고 있는지?
	UPROPERTY(BlueprintReadOnly)
		bool bIsRolling = false;

	//현재 공격을 하고 있는지?
	UPROPERTY(BlueprintReadOnly)
		bool bIsAttacking = false;;

	//PlayerMaxHP는 1.0f
	UPROPERTY(BlueprintReadOnly)
		float CharacterCurrHP;
};

UENUM(BlueprintType)
enum class ECharacterDebuffType : uint8
{
	E_Flame		UMETA(DisplayName = "Flame"),
	E_Poison	UMETA(DisplayName = "Poison"),
	E_Slow		UMETA(DisplayName = "Slow"),
	E_Sleep		UMETA(DisplayName = "Sleep"),
	//방어력 감소
	//공격력 감소
};

UENUM(BlueprintType)
enum class ECharacterBuffType : uint8
{
	E_SpeedUp		UMETA(DisplayName = "SpeedUp"),
	E_Healing		UMETA(DisplayName = "Healing"),
	E_FastAtk		UMETA(DisplayName = "FastAtk")
};