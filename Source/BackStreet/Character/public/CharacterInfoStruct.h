#pragma once

#include "Engine/DataTable.h"
#include "CharacteInfoEnum.h"
#include "CharacterInfoStruct.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatStruct : public FTableRowBase
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
		bool bIsInvincibility = false;

	UPROPERTY(BlueprintReadOnly)
		bool bInfiniteAmmo = false;

	//PlayerMaxHP�� 1.0f
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0.5f, UIMax = 10.0f))
		float CharacterMaxHP = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0.1f, UIMax = 10.0f))
		float CharacterAtkMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0.2f, UIMax = 1.0f))
		float CharacterAtkSpeed = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 100.0f, UIMax = 1000.0f))
		float CharacterMoveSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = -1.0f, UIMax = 1.0f))
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

	//������ �� �� �ִ� ��������?
	UPROPERTY(BlueprintReadOnly)
		bool bCanAttack = false;

	//0 : Idle,  1 : Left Turn,  2 : Right Turn
	UPROPERTY(BlueprintReadOnly)
		uint8 TurnDirection = 0;

	//ĳ������ �ൿ ����
	UPROPERTY(BlueprintReadWrite)
		ECharacterActionType CharacterActionState;

	//PlayerMaxHP�� 1.0f
	UPROPERTY(BlueprintReadOnly)
		float CharacterCurrHP;
};