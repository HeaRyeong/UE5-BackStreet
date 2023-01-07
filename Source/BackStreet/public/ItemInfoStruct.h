#pragma once

#include "Engine/DataTable.h"
#include "ItemInfoStruct.generated.h"


USTRUCT(BlueprintType)
struct FBuffItemInfoStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName BuffName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Value;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Time;

};