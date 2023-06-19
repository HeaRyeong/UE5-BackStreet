// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/DebuffManager.h"
#include "../public/BackStreetGameModeBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Item/public/WeaponInventoryBase.h"

#define MAX_DEBUFF_IDX 7
#define DEBUFF_DAMAGE_TIMER_IDX 15
#define MAX_DEBUFF_TIME 150.0f

//새로운 타이머 핸들 리스트를 생성할 때 복사할 리스트
TArray<FTimerHandle> TempTimerHandleList;
TArray<float> TempResetValueList;

UDebuffManager::UDebuffManager()
{

}

bool UDebuffManager::SetDebuffTimer(ECharacterDebuffType DebuffType, ACharacterBase* Target, AActor* Causer, float TotalTime, float Variable)
{
	UE_LOG(LogTemp, Warning, TEXT("Try!"));
	if (!IsValid(GamemodeRef) || !IsValid(Target)) return false;

	FTimerDelegate timerDelegate, healTimerDelegate, dotDamageDelegate;
	FTimerHandle& timerHandle = GetDebuffTimerHandleRef(DebuffType, Target);

	FCharacterStateStruct characterState = Target->GetCharacterState();
	FCharacterStatStruct characterStat = Target->GetCharacterStat();

	if (GetDebuffIsActive((ECharacterDebuffType)DebuffType, Target))
	{
		UE_LOG(LogTemp, Warning, TEXT("Re-Apply"));

		float& resetValue = (GetResetValueListRef(Target))[GetDebuffInfoListIndex(DebuffType)];
		float remainTime = GamemodeRef->GetWorldTimerManager().GetTimerRemaining(timerHandle);

		ResetStatDebuffState(DebuffType, Target, resetValue);
		resetValue = 0.0f;

		return SetDebuffTimer( DebuffType, Target, Causer, FMath::Min(TotalTime + remainTime, MAX_DEBUFF_TIME), Variable);
	}

	/*---- 디버프 타이머 세팅 ----------------------------*/
	Variable = FMath::Min(1.0f, FMath::Abs(Variable)); //값 정제
	characterState.CharacterDebuffState |= (1 << (int)DebuffType);

	UE_LOG(LogTemp, Warning, TEXT("Switch"));

	switch ((ECharacterDebuffType)DebuffType)
	{
		//----데미지 디버프-------------------
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			dotDamageDelegate.BindUFunction(Target, FName("TakeDebuffDamage"), Variable, DebuffType, Causer);
			if (!(GetTimerHandleListRef(Target)).IsValidIndex(DEBUFF_DAMAGE_TIMER_IDX))
			{
				UE_LOG(LogTemp, Warning, TEXT("Timer Handle List is Invalid!!! - - - - - "));
			}
			else 
				GamemodeRef->GetWorldTimerManager().SetTimer((GetTimerHandleListRef(Target))[DEBUFF_DAMAGE_TIMER_IDX], dotDamageDelegate, 1.0f, true);
			
			break;
		//----스탯 조정 디버프-------------------
		case ECharacterDebuffType::E_Stun:
			Target->StopAttack();
			characterState.CharacterActionState = ECharacterActionType::E_Stun;
			Target->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			UE_LOG(LogTemp, Warning, TEXT("STUN ACTIVATED"));

			break;
		case ECharacterDebuffType::E_Slow:
			characterStat.CharacterMoveSpeed *= Variable;
			characterStat.CharacterAtkSpeed *= Variable;
			break;
		case ECharacterDebuffType::E_AttackDown:
			characterStat.CharacterAtkMultiplier *= Variable;
			break;
		case ECharacterDebuffType::E_DefenseDown:
			characterStat.CharacterDefense *= Variable;
			break;
	}
	(GetResetValueListRef(Target))[GetDebuffInfoListIndex(DebuffType)] = Variable;

	Target->UpdateCharacterStat(characterStat);
	Target->UpdateCharacterState(characterState);

	UE_LOG(LogTemp, Warning, TEXT("Set"));

	timerDelegate.BindUFunction(this, FName("ResetStatDebuffState"),  DebuffType, Target, Variable);
	GamemodeRef->GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 0.1f, false, TotalTime);

	UE_LOG(LogTemp, Warning, TEXT("Timer"));

	return true;
}

void UDebuffManager::ResetStatDebuffState(ECharacterDebuffType DebuffType, ACharacterBase* Target, float ResetVal)
{
	if (!IsValid(Target)) return;
	FCharacterStateStruct characterState = Target->GetCharacterState();
	FCharacterStatStruct characterStat = Target->GetCharacterStat();

	ResetVal = FMath::Max(0.0f, ResetVal);
	characterState.CharacterDebuffState &= ~(1 << (int)DebuffType);

	switch ((ECharacterDebuffType)DebuffType)
	{
	case ECharacterDebuffType::E_Flame:
	case ECharacterDebuffType::E_Poison:
		GamemodeRef->GetWorldTimerManager().ClearTimer((GetTimerHandleListRef(Target))[DEBUFF_DAMAGE_TIMER_IDX]);
		break;
	case ECharacterDebuffType::E_Slow:
		characterStat.CharacterMoveSpeed /= ResetVal;
		characterStat.CharacterAtkSpeed /= ResetVal;
		break;
	case ECharacterDebuffType::E_Stun:
		UE_LOG(LogTemp, Warning, TEXT("TRY STUN DEACTIVATED"));
		characterState.CharacterActionState = ECharacterActionType::E_Idle;
		Target->ResetActionState(true);
		Target->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		break;
	case ECharacterDebuffType::E_AttackDown:
		characterStat.CharacterAtkMultiplier /= ResetVal;
		break;
	case ECharacterDebuffType::E_DefenseDown:
		characterStat.CharacterDefense /= ResetVal;
		break;
	}
	
	Target->UpdateCharacterStat(characterStat);
	Target->UpdateCharacterState(characterState);
	ClearDebuffTimer(DebuffType, Target);
}

void UDebuffManager::ClearDebuffTimer(ECharacterDebuffType DebuffType, ACharacterBase* Target)
{
	GamemodeRef->GetWorldTimerManager().ClearTimer(GetDebuffTimerHandleRef(DebuffType, Target));
}

void UDebuffManager::ClearAllDebuffTimer()
{
	const uint16 startIdx = 0;
	const uint16 endIdx = MAX_DEBUFF_IDX;
	
	for (auto timerListIterator = TimerInfoMap.CreateIterator(); timerListIterator; ++timerListIterator)
	{
		TArray<FTimerHandle>& timerListRef = timerListIterator->Value.TimerHandleList;
		for (auto& targetTimer : timerListRef)
		{
			GamemodeRef->GetWorldTimerManager().ClearTimer(targetTimer);
		}
		timerListIterator->Value.ResetValueHandleList.Empty();
		timerListIterator->Value.TimerHandleList.Empty();
	}
}

void UDebuffManager::InitDebuffManager(ABackStreetGameModeBase* NewGamemodeRef)
{
	if (!IsValid(NewGamemodeRef)) return;

	for (int newTimerIdx = 0; newTimerIdx <= MAX_DEBUFF_IDX; newTimerIdx += 1)
	{
		TempTimerHandleList.Add(FTimerHandle());
		TempResetValueList.Add(0.0f);
	}
	GamemodeRef = NewGamemodeRef;
}

bool UDebuffManager::GetDebuffIsActive(ECharacterDebuffType DebuffType, ACharacterBase* Target)
{
	if (!IsValid(Target)) return false;
//	if (OwnerCharacterRef->GetCharacterState().CharacterDebuffState & (1 << (int)DebuffType)) return true;
	return false;
}

FTimerHandle& UDebuffManager::GetDebuffTimerHandleRef(ECharacterDebuffType DebuffType, ACharacterBase* Target)
{
	TArray<FTimerHandle>& timerHandleListRef = GetTimerHandleListRef(Target);
	int16 targetListIdx = GetDebuffInfoListIndex(DebuffType);
	return timerHandleListRef[targetListIdx];
}

int16 UDebuffManager::GetDebuffInfoListIndex(ECharacterDebuffType DebuffType)
{
	return (uint16)DebuffType;
}


TArray<FTimerHandle>& UDebuffManager::GetTimerHandleListRef(ACharacterBase* Target)
{
	if (!TimerInfoMap.Contains(Target->GetUniqueID()))
	{
		AddNewTimerList(Target);
	}
	return TimerInfoMap[Target->GetUniqueID()].TimerHandleList;
}

TArray<float>& UDebuffManager::GetResetValueListRef(ACharacterBase* Target)
{
	if (!TimerInfoMap.Contains(Target->GetUniqueID()))
	{
		AddNewTimerList(Target);
	}
	return TimerInfoMap[Target->GetUniqueID()].ResetValueHandleList;
}

void UDebuffManager::AddNewTimerList(ACharacterBase* Target)
{
	if (!IsValid(Target)) return;
	FDebuffTimerInfoStruct newTimerInfo = { TempTimerHandleList, TempResetValueList };
	TimerInfoMap.Add({ Target->GetUniqueID(), newTimerInfo });
}