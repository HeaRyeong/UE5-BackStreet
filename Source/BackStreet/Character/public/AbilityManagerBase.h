// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "AbilityManagerBase.generated.h"

UCLASS()
class BACKSTREET_API AAbilityManagerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAbilityManagerBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		bool TryAddNewAbility(ECharacterAbilityType NewAbilityType);

	UFUNCTION()
		bool RemoveAbility(ECharacterAbilityType TargetAbilityType);

	UFUNCTION()
		bool TryActivateAbility(ECharacterAbilityType TargetAbilityType);

	UFUNCTION()
		void ClearAllAbility();

protected:
	//최대 어빌리티 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 1, UIMax = 5))
		int32 MaxAbilityCount = 3;

	

private:
	//현재 플레이어가 소유한 어빌리티의 
	UPROPERTY()
		TSet<ECharacterAbilityType> AbilityList;

	UPROPERTY()
		int32 CurrentAbilityCount = 0;
};
