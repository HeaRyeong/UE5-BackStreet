// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/AbilityManagerBase.h"
#include "../../Character/public/CharacterBase.h"

// Sets default values
UAbilityManagerBase::UAbilityManagerBase()
{
	
}

void UAbilityManagerBase::InitAbilityManager(ACharacterBase* NewCharacter)
{
	if (!IsValid(NewCharacter)) return;
	UE_LOG(LogTemp, Warning, TEXT("Initialize Ability Manager Success"));
	OwnerCharacterRef = NewCharacter;
}

bool UAbilityManagerBase::TryAddNewAbility(const ECharacterAbilityType NewAbilityType)
{
	if (!IsValid(OwnerCharacterRef)) return false; 
	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();

	if (GetIsAbilityActive(NewAbilityType)) return false;
	if(ActiveAbilityList.Num() >= MaxAbilityCount) return false;

	FAbilityInfoStruct newAbilityInfo = GetAbilityInfo(NewAbilityType);
	newAbilityInfo.AbilityId = (uint8)NewAbilityType;
	if (newAbilityInfo.AbilityId == -1) return false;

	//레거시 코드(...)
	switch (NewAbilityType)
	{
	//----힐 버프-------------------
	case ECharacterAbilityType::E_Healing:
		newAbilityInfo.TimerDelegate.BindUFunction(OwnerCharacterRef, FName("TakeHeal"), 1.0f, true, NewAbilityType);
		OwnerCharacterRef->GetWorldTimerManager().SetTimer(newAbilityInfo.TimerHandle, newAbilityInfo.TimerDelegate, 1.0f, true);
		break;
	//----스탯 조정 버프-------------------
	case ECharacterAbilityType::E_AttackUp:
		characterStat.CharacterAtkMultiplier *= 1.25f;
		break;
	case ECharacterAbilityType::E_DefenseUp:
		characterStat.CharacterDefense *= 1.25f;
		break;
	case ECharacterAbilityType::E_SpeedUp:
		characterStat.CharacterMoveSpeed *= 1.25f;
		characterStat.CharacterAtkSpeed *= 1.25f;
		break;
	case ECharacterAbilityType::E_Invincibility:
		characterStat.bIsInvincibility = true;
		break;
	case ECharacterAbilityType::E_Infinite:
		characterStat.bInfinite = true;
		break;
	case ECharacterAbilityType::E_DoubleShot:
		characterStat.MaxProjectileCount = 3;
		break;
	case ECharacterAbilityType::E_TripleShot:
		characterStat.MaxProjectileCount = 3;
		break;
	}
	OwnerCharacterRef->UpdateCharacterStat(characterStat);
	OwnerCharacterRef->UpdateCharacterState(characterState);
	ActiveAbilityList.Add(newAbilityInfo);

	return true;
}

bool UAbilityManagerBase::TryRemoveAbility(const ECharacterAbilityType TargetAbilityType)
{
	if (!IsValid(OwnerCharacterRef)) return false;
	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();

	if (!GetIsAbilityActive(TargetAbilityType)) return false;
	if (ActiveAbilityList.Num() >= MaxAbilityCount) return false;

	FAbilityInfoStruct targetAbilityInfo = GetAbilityInfo(TargetAbilityType);
	targetAbilityInfo.AbilityId = (uint8)TargetAbilityType;
	if (targetAbilityInfo.AbilityId == -1) return false;
	
	for (int idx = 0; idx < ActiveAbilityList.Num(); idx++)
	{
		const FAbilityInfoStruct& abilityInfo = ActiveAbilityList[idx];
		if (abilityInfo.AbilityId == (uint8)TargetAbilityType)
			ActiveAbilityList.RemoveAt(idx);
	}

	//레거시 코드(...)
	switch (TargetAbilityType)
	{
	case ECharacterAbilityType::E_Healing:
		OwnerCharacterRef->GetWorldTimerManager().ClearTimer(
			targetAbilityInfo.TimerHandle
		);
		break;
	case ECharacterAbilityType::E_DefenseUp:
		characterStat.CharacterDefense /= 1.25f;
		break;
	case ECharacterAbilityType::E_AttackUp:
		characterStat.CharacterAtkMultiplier /= 1.25f;
		break;
	case ECharacterAbilityType::E_SpeedUp:
		characterStat.CharacterMoveSpeed /= 1.25f;
		characterStat.CharacterAtkSpeed /= 1.25f;
		break;
	case ECharacterAbilityType::E_Invincibility:
		characterStat.bIsInvincibility = false;
		break;
	case ECharacterAbilityType::E_Infinite:
		characterStat.bInfinite = false;
		break;
	case ECharacterAbilityType::E_DoubleShot:
	case ECharacterAbilityType::E_TripleShot:
		characterStat.MaxProjectileCount = 1;
		break; 
	}

	return true;
}

void UAbilityManagerBase::ClearAllAbility()
{
	ActiveAbilityList.Empty();
}

bool UAbilityManagerBase::GetIsAbilityActive(const ECharacterAbilityType NewAbilityType)
{
	for (FAbilityInfoStruct& abilityInfo : ActiveAbilityList)
	{
		if (abilityInfo.AbilityId == (uint8)NewAbilityType)
		{
			return true;
		}
	}
	return false;
}

FAbilityInfoStruct UAbilityManagerBase::GetAbilityInfo(const ECharacterAbilityType AbilityType)
{
	for (FAbilityInfoStruct& abilityInfo : ActiveAbilityList)
	{
		if (abilityInfo.AbilityId == (uint8)AbilityType)
		{
			return abilityInfo;
		}
	}
	return FAbilityInfoStruct();
}
