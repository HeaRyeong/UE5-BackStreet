// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/AbilityManagerBase.h"
#include "../../Character/public/CharacterBase.h"

// Sets default values
UAbilityManagerBase::UAbilityManagerBase()
{
	
}

void UAbilityManagerBase::InitAbilityManager()
{
	if (IsValid(GetOuter())) return;
	OwnerCharacterRef = Cast<ACharacterBase>(GetOuter());
}

bool UAbilityManagerBase::TryAddNewAbility(ECharacterAbilityType NewAbilityType)
{
	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();
	FTimerDelegate healTimerDelegate;

	CharacterAbilityState |= (1 << (int)NewAbilityType);

	switch ((ECharacterAbilityType)NewAbilityType)
	{
		//----힐 버프-------------------
	case ECharacterAbilityType::E_Healing:
		healTimerDelegate.BindUFunction(OwnerCharacterRef, FName("TakeHeal"), 1.0f, true, NewAbilityType);
		OwnerCharacterRef->GetWorldTimerManager().SetTimer(HealTimerHandle, healTimerDelegate, 1.0f, true);
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
	}
	OwnerCharacterRef->UpdateCharacterStat(characterStat);
	OwnerCharacterRef->UpdateCharacterState(characterState);

	return false;
}

bool UAbilityManagerBase::RemoveAbility(ECharacterAbilityType TargetAbilityType)
{
	CharacterAbilityState &= ~(1 << (int)TargetAbilityType);
	return false;
}

void UAbilityManagerBase::ClearAllAbility()
{
	CharacterAbilityState = (1 << 10);
}