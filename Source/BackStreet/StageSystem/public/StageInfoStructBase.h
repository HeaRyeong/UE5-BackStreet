#pragma once

#include "Engine/DataTable.h"
#include "StageInfoStructBase.generated.h"

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