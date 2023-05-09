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

	//초기화 시점에 진행
	UDataTable* abilityInfoTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Character/MainCharacter/Data/D_AbilityInfoDataTable.D_AbilityInfoDataTable'"));
	if (!InitAbilityInfoListFromTable(abilityInfoTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAbilityManagerBase::InitAbilityManager) DataTable is not found!"));
	}
}

bool UAbilityManagerBase::TryAddNewAbility(const ECharacterAbilityType NewAbilityType)
{
	if (!IsValid(OwnerCharacterRef)) return false; 
	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();

	if (GetIsAbilityActive(NewAbilityType)) return false;
	if (ActiveAbilityList.Num() >= MaxAbilityCount) return false;

	FAbilityInfoStruct newAbilityInfo = GetAbilityInfo(NewAbilityType);
	if (newAbilityInfo.AbilityId == -1) return false;

	if (newAbilityInfo.bIsRepetitive)
	{
		const float variable = newAbilityInfo.Variable.Num() > 0 ? newAbilityInfo.Variable[0] : 1.0f;
		newAbilityInfo.TimerDelegate.BindUFunction(OwnerCharacterRef, newAbilityInfo.FuncName, variable, true, (uint8)NewAbilityType);
		OwnerCharacterRef->GetWorldTimerManager().SetTimer(newAbilityInfo.TimerHandle, newAbilityInfo.TimerDelegate, 1.0f, true);
	}
	TryUpdateCharacterStat(newAbilityInfo, false);
	ActiveAbilityList.Add(newAbilityInfo);

	return true;
}

bool UAbilityManagerBase::TryRemoveAbility(const ECharacterAbilityType TargetAbilityType)
{
	if (!IsValid(OwnerCharacterRef)) return false;
	
	if (!GetIsAbilityActive(TargetAbilityType)) return false;
	if (ActiveAbilityList.Num() >= MaxAbilityCount) return false;

	FAbilityInfoStruct targetAbilityInfo = GetAbilityInfo(TargetAbilityType);
	targetAbilityInfo.AbilityId = (uint8)TargetAbilityType;
	if (targetAbilityInfo.AbilityId == -1) return false;
	
	for (int idx = 0; idx < ActiveAbilityList.Num(); idx++)
	{
		FAbilityInfoStruct& abilityInfo = ActiveAbilityList[idx];
		if (abilityInfo.AbilityId == (uint8)TargetAbilityType)
		{
			TryUpdateCharacterStat(abilityInfo, true);
			ActiveAbilityList.RemoveAt(idx);
			if (abilityInfo.bIsRepetitive)
			{
				OwnerCharacterRef->GetWorldTimerManager().ClearTimer(abilityInfo.TimerHandle);
			}
			return true;
		}
	}
	return false;
}

void UAbilityManagerBase::ClearAllAbility()
{
	ActiveAbilityList.Empty();
}

bool UAbilityManagerBase::TryUpdateCharacterStat(const FAbilityInfoStruct TargetAbilityInfo, bool bIsReset)
{
	//Validity 체크 (꺼져있는데 제거를 시도하거나, 켜져있는데 추가를 시도한다면?)
	if (GetIsAbilityActive((ECharacterAbilityType)TargetAbilityInfo.AbilityId) != bIsReset) return false;
	
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();
	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();

	for (int statIdx = 0; statIdx < TargetAbilityInfo.TargetStatName.Num(); statIdx++)
	{
		FName targetStatName = TargetAbilityInfo.TargetStatName[statIdx];
		float targetVariable = TargetAbilityInfo.Variable[FMath::Min(TargetAbilityInfo.Variable.Num() - 1, statIdx)];
		if (bIsReset) targetVariable = 1/targetVariable; //초기화 시 1보다 낮은 값으로 곱함 1.25 vs 0.25

		if (targetStatName == FName("MaxHP"))
		{
			characterStat.CharacterMaxHP *= targetVariable;
			characterState.CharacterCurrHP = FMath::Min(characterStat.CharacterMaxHP, characterState.CharacterCurrHP);
		}
		//else if (targetStatName == FName("CurrHP"))
		//	characterState.CharacterCurrHP = characterStat.CharacterMaxHP;
		else if (targetStatName == FName("Attack"))
			characterStat.CharacterAtkMultiplier *= targetVariable;
		else if (targetStatName == FName("Defense"))
			characterStat.CharacterDefense *= targetVariable;
		else if (targetStatName == FName("MoveSpeed"))
			characterStat.CharacterMoveSpeed *= targetVariable;
		else if (targetStatName == FName("AttackSpeed"))
			characterStat.CharacterAtkSpeed *= targetVariable;
		else if (targetStatName == FName("MaxProjectileCount"))
			characterStat.MaxProjectileCount *= targetVariable;
	}
	OwnerCharacterRef->UpdateCharacterStat(characterStat);
	OwnerCharacterRef->UpdateCharacterState(characterState);

	return true;
}

bool UAbilityManagerBase::GetIsAbilityActive(const ECharacterAbilityType TargetAbilityType)
{
	for (FAbilityInfoStruct& abilityInfo : ActiveAbilityList)
	{
		if (abilityInfo.AbilityId == (uint8)TargetAbilityType)
		{
			return true;
		}
	}
	return false;
}

FAbilityInfoStruct UAbilityManagerBase::GetAbilityInfo(const ECharacterAbilityType AbilityType)
{
	if(!AbilityInfoList.IsValidIndex((uint8)AbilityType)) return FAbilityInfoStruct();
	return AbilityInfoList[(uint8)AbilityType];
}

bool UAbilityManagerBase::InitAbilityInfoListFromTable(const UDataTable* AbilityInfoTable)
{
	if (AbilityInfoTable == nullptr) return false;

	const TArray<FName> rowNameList = AbilityInfoTable->GetRowNames();
	AbilityInfoList.Empty();
	AbilityInfoList.Add(FAbilityInfoStruct());
	for (const FName& rowName : rowNameList)
	{
		FAbilityInfoStruct* abilityInfo = AbilityInfoTable->FindRow<FAbilityInfoStruct>(rowName, "");
		if (abilityInfo != nullptr)
		{
			AbilityInfoList.Add(*abilityInfo);
		}
	}
	return true;
}

// 임시 코드
TArray<ECharacterAbilityType> UAbilityManagerBase::GetActiveAbilityList()
{
	TArray<ECharacterAbilityType> returnActiveAbility;
	for (FAbilityInfoStruct& abilityInfo : ActiveAbilityList)
	{
		returnActiveAbility.Add((const ECharacterAbilityType)abilityInfo.AbilityId);
	}
	return returnActiveAbility;
}
