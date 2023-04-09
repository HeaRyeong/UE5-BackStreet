// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "AbilityManagerBase.generated.h"

UCLASS()
class BACKSTREET_API UAbilityManagerBase : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	UAbilityManagerBase();

public:
	UFUNCTION()
		void InitAbilityManager();

	UFUNCTION()
		bool TryAddNewAbility(ECharacterAbilityType NewAbilityType);

	UFUNCTION()
		bool RemoveAbility(ECharacterAbilityType TargetAbilityType);

	UFUNCTION()
		void ClearAllAbility();

protected:
	//최대 어빌리티 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 1, UIMax = 5))
		int32 MaxAbilityCount = 3;

private:
	//현재 플레이어가 소유한 어빌리티의 종류
	UPROPERTY()
		int32 CharacterAbilityState = (1 << 10);

	UPROPERTY()
		TArray<float> ResetStatValueList;

	UPROPERTY()
		int32 CurrentAbilityCount = 0;

	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		FTimerHandle HealTimerHandle;
};
