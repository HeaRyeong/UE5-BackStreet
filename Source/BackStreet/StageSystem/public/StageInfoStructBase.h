#pragma once

#include "Engine/DataTable.h"
#include "StageInfoStructBase.generated.h"

UENUM(BlueprintType)
enum class EStageCategoryInfo : uint8
{
	E_None				  	UMETA(DisplayName = "None"),
	E_Normal				UMETA(DisplayName = "Normal"),
	E_Mission			  	UMETA(DisplayName = "Mission"),
	E_Boss				    UMETA(DisplayName = "Boss"),
};

USTRUCT(BlueprintType)
struct FStageEnemyRankStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		uint8 StageLevel;
	UPROPERTY(EditAnywhere)
		FName StageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.5f, UIMax = 10.0f))
		float CharacterMaxHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.1f, UIMax = 10.0f))
		float CharacterAtkMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.2f, UIMax = 1.0f))
		float CharacterAtkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 100.0f, UIMax = 1000.0f))
		float CharacterMoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = -1.0f, UIMax = 1.0f))
		float CharacterDefense;

};

USTRUCT(BlueprintType)
struct FStageEnemyTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		FName StageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ID_1001;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ID_1002;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ID_1003;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ID_1100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ID_1101;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ID_1102;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ID_1200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MinSpawn;


};

