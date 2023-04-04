// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/AbilityManagerBase.h"

// Sets default values
AAbilityManagerBase::AAbilityManagerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAbilityManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AAbilityManagerBase::TryAddNewAbility(ECharacterAbilityType NewAbility)
{
	return false;
}

bool AAbilityManagerBase::RemoveAbility(ECharacterAbilityType TargetAbilityType)
{
	return false;
}

bool AAbilityManagerBase::TryActivateAbility(ECharacterAbilityType TargetAbilityType)
{
	return false;
}

void AAbilityManagerBase::ClearAllAbility()
{

}