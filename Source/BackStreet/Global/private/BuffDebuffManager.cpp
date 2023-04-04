// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/BuffDebuffManager.h"
#include "../public/BackStreetGameModeBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Item/public/WeaponInventoryBase.h"

#define MAX_BUFF_IDX 6
#define MAX_DEBUFF_IDX 7
#define HEAL_BUFF_TIMER_IDX 16
#define DEBUFF_DAMAGE_TIMER_IDX 15
#define MAX_BUFF_INFO_LIST_IDX 24
#define MAX_BUFF_DEBUFF_TIME 150.0f

//새로운 타이머 핸들 리스트를 생성할 때 복사할 리스트
TArray<FTimerHandle> TempTimerHandleList;
TArray<float> TempResetValueList;

UBuffDebuffManager::UBuffDebuffManager()
{

}

bool UBuffDebuffManager::SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, ACharacterBase* Target, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(GamemodeRef) || !IsValid(Target)) return false;

	FTimerDelegate timerDelegate, healTimerDelegate, dotDamageDelegate;
	FTimerHandle& timerHandle = GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType, Target);

	FCharacterStateStruct characterState = Target->GetCharacterState();
	FCharacterStatStruct characterStat = Target->GetCharacterStat();

	if ((bIsDebuff && GetDebuffIsActive((ECharacterDebuffType)BuffDebuffType, Target))
		|| (!bIsDebuff && GetBuffIsActive((ECharacterBuffType)BuffDebuffType, Target)))
	{
		float& resetValue = (GetResetValueListRef(Target))[GetBuffDebuffInfoListIndex(bIsDebuff, BuffDebuffType)];
		float remainTime = GamemodeRef->GetWorldTimerManager().GetTimerRemaining(timerHandle);

		ResetStatBuffDebuffState(bIsDebuff, BuffDebuffType, Target, resetValue);
		resetValue = 0.0f;

		return SetBuffDebuffTimer(bIsDebuff, BuffDebuffType, Target, Causer, FMath::Min(TotalTime + remainTime, MAX_BUFF_DEBUFF_TIME), Variable);
	}

	/*---- 디버프 타이머 세팅 ----------------------------*/
	if (bIsDebuff)
	{
		Variable = FMath::Min(1.0f, FMath::Abs(Variable)); //값 정제
		characterState.CharacterDebuffState |= (1 << (int)BuffDebuffType);

		switch ((ECharacterDebuffType)BuffDebuffType)
		{
		//----데미지 디버프-------------------
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			dotDamageDelegate.BindUFunction(Target, FName("TakeDebuffDamage"), Variable, BuffDebuffType, Causer);
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
		(GetResetValueListRef(Target))[GetBuffDebuffInfoListIndex(bIsDebuff, BuffDebuffType)] = Variable;

		Target->UpdateCharacterStat(characterStat);
		Target->UpdateCharacterState(characterState);

		timerDelegate.BindUFunction(this, FName("ResetStatBuffDebuffState"), bIsDebuff, BuffDebuffType, Target, Variable);
		GamemodeRef->GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 0.1f, false, TotalTime);

		return true;
	}

	/*---- 버프 타이머 세팅 ----------------------------*/
	else
	{
		Variable = 1.0f + FMath::Abs(Variable); //값 정제
		characterState.CharacterBuffState |= (1 << (int)BuffDebuffType);

		switch ((ECharacterBuffType)BuffDebuffType)
		{
		//----힐 버프-------------------
		case ECharacterBuffType::E_Healing:
			Variable -= 1.0f;
			healTimerDelegate.BindUFunction(Target, FName("TakeHeal"), Variable, true, BuffDebuffType);
			GamemodeRef->GetWorldTimerManager().SetTimer((GetTimerHandleListRef(Target))[HEAL_BUFF_TIMER_IDX], healTimerDelegate, 1.0f, true);
			break;
			//----스탯 조정 버프-------------------
		case ECharacterBuffType::E_AttackUp:
			characterStat.CharacterAtkMultiplier *= Variable;
			break;
		case ECharacterBuffType::E_DefenseUp:
			characterStat.CharacterDefense *= Variable;
			break;
		case ECharacterBuffType::E_SpeedUp:
			characterStat.CharacterMoveSpeed *= Variable;
			characterStat.CharacterAtkSpeed *= Variable;
			break;
		case ECharacterBuffType::E_Invincibility:
			characterStat.bIsInvincibility = true;
			break;
		case ECharacterBuffType::E_Infinite:
			if (characterStat.bInfinite) return false; //스탯으로써 무한 능력을 갖고 있다면?
			characterStat.bInfinite = true;
			break;
		}
		(GetResetValueListRef(Target))[GetBuffDebuffInfoListIndex(bIsDebuff, BuffDebuffType)] = Variable;

		timerDelegate.BindUFunction(this, FName("ResetStatBuffDebuffState"), bIsDebuff, BuffDebuffType, Target, Variable);
		GamemodeRef->GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 0.1f, false, TotalTime);

		Target->UpdateCharacterStat(characterStat);
		Target->UpdateCharacterState(characterState);

		return true;
	}
	return false;
}

void UBuffDebuffManager::ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, ACharacterBase* Target, float ResetVal)
{
	if (!IsValid(Target)) return;
	FCharacterStateStruct characterState = Target->GetCharacterState();
	FCharacterStatStruct characterStat = Target->GetCharacterStat();

	if (bIsDebuff)
	{
		ResetVal = FMath::Max(0.0f, ResetVal);
		characterState.CharacterDebuffState &= ~(1 << (int)BuffDebuffType);

		switch ((ECharacterDebuffType)BuffDebuffType)
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
	}
	else
	{
		characterState.CharacterBuffState &= ~(1 << (int)BuffDebuffType);

		switch ((ECharacterBuffType)BuffDebuffType)
		{
		case ECharacterBuffType::E_Healing:
			GamemodeRef->GetWorldTimerManager().ClearTimer((GetTimerHandleListRef(Target))[HEAL_BUFF_TIMER_IDX]);
			break;
		case ECharacterBuffType::E_DefenseUp:
			characterStat.CharacterDefense /= ResetVal;
			break;
		case ECharacterBuffType::E_AttackUp:
			characterStat.CharacterAtkMultiplier /= ResetVal;
			break;
		case ECharacterBuffType::E_SpeedUp:
			characterStat.CharacterMoveSpeed /= ResetVal;
			characterStat.CharacterAtkSpeed /= ResetVal;
			break;
		case ECharacterBuffType::E_Invincibility:
			characterStat.bIsInvincibility = false;
			break;
		case ECharacterBuffType::E_Infinite:
			characterStat.bInfinite = false;
			break;
		}
	}
	Target->UpdateCharacterStat(characterStat);
	Target->UpdateCharacterState(characterState);
	ClearBuffDebuffTimer(bIsDebuff, BuffDebuffType, Target);
}

void UBuffDebuffManager::ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, ACharacterBase* Target)
{
	GamemodeRef->GetWorldTimerManager().ClearTimer(GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType, Target));
}

void UBuffDebuffManager::ClearAllBuffDebuffTimer(bool bIsDebuff)
{
	const uint16 startIdx = bIsDebuff ? MAX_BUFF_IDX + 2 : 0;
	const uint16 endIdx = bIsDebuff ? startIdx + MAX_DEBUFF_IDX : MAX_BUFF_IDX + 1;
	
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

void UBuffDebuffManager::InitBuffManager(ABackStreetGameModeBase* NewGamemodeRef)
{
	UE_LOG(LogTemp, Warning, TEXT("TryActivate BuffManager #2"));
	if (!IsValid(NewGamemodeRef)) return;

	UE_LOG(LogTemp, Warning, TEXT("TryActivate BuffManager #3"));

	for (int newTimerIdx = 0; newTimerIdx <= MAX_BUFF_INFO_LIST_IDX; newTimerIdx += 1)
	{
		TempTimerHandleList.Add(FTimerHandle());
		TempResetValueList.Add(0.0f);
	}
	GamemodeRef = NewGamemodeRef;
}

bool UBuffDebuffManager::SetBuffTimer(ECharacterBuffType BuffType, ACharacterBase* Target, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	return SetBuffDebuffTimer(false, (uint8)BuffType, Target, Causer, TotalTime, Variable);
}

bool UBuffDebuffManager::SetDebuffTimer(ECharacterDebuffType DebuffType, ACharacterBase* Target, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	return SetBuffDebuffTimer(true, (uint8)DebuffType, Target, Causer, TotalTime, Variable);
}

bool UBuffDebuffManager::GetDebuffIsActive(ECharacterDebuffType DebuffType, ACharacterBase* Target)
{
	if (!IsValid(Target)) return false;
//	if (OwnerCharacterRef->GetCharacterState().CharacterDebuffState & (1 << (int)DebuffType)) return true;
	return false;
}

bool UBuffDebuffManager::GetBuffIsActive(ECharacterBuffType BuffType, ACharacterBase* Target)
{
	if (!IsValid(Target)) return false;
//	if (OwnerCharacterRef->GetCharacterState().CharacterBuffState & (1 << (int)BuffType)) return true;
	return false;
}

float UBuffDebuffManager::GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType, ACharacterBase* Target)
{
	FTimerHandle& targetBuffTimer = GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType, Target);
	return GamemodeRef->GetWorldTimerManager().GetTimerRemaining(targetBuffTimer);
}

FTimerHandle& UBuffDebuffManager::GetBuffDebuffTimerHandleRef(bool bIsDebuff, uint8 BuffDebuffType, ACharacterBase* Target)
{
	TArray<FTimerHandle>& timerHandleListRef = GetTimerHandleListRef(Target);
	int16 targetListIdx = GetBuffDebuffInfoListIndex(bIsDebuff, BuffDebuffType);
	return timerHandleListRef[targetListIdx];
}

int16 UBuffDebuffManager::GetBuffDebuffInfoListIndex(bool bIsDebuff, uint8 BuffDebuffType)
{
	return bIsDebuff ? BuffDebuffType + MAX_BUFF_IDX + 1 : BuffDebuffType;
}

TArray<FTimerHandle>& UBuffDebuffManager::GetTimerHandleListRef(ACharacterBase* Target)
{
	if (!TimerInfoMap.Contains(Target->GetUniqueID()))
	{
		AddNewTimerList(Target);
	}
	return TimerInfoMap[Target->GetUniqueID()].TimerHandleList;
}

TArray<float>& UBuffDebuffManager::GetResetValueListRef(ACharacterBase* Target)
{
	if (!TimerInfoMap.Contains(Target->GetUniqueID()))
	{
		AddNewTimerList(Target);
	}
	return TimerInfoMap[Target->GetUniqueID()].ResetValueHandleList;
}

void UBuffDebuffManager::AddNewTimerList(ACharacterBase* Target)
{
	if (!IsValid(Target)) return;
	FBuffDebuffTimerInfoStruct newTimerInfo = { TempTimerHandleList, TempResetValueList };

	TimerInfoMap.Add({ Target->GetUniqueID(), newTimerInfo });
}
