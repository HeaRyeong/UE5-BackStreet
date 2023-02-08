// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/CharacterBuffManager.h"
#include "../public/CharacterBase.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Item/public/WeaponInventoryBase.h"

#define MAX_BUFF_IDX 6
#define MAX_DEBUFF_IDX 7
#define HEAL_BUFF_TIMER_IDX 6
#define DEBUFF_DAMAGE_TIMER_IDX 15

ACharacterBuffManager::ACharacterBuffManager()
{
	SetActorTickEnabled(false);
}

void ACharacterBuffManager::BeginPlay()
{
	if (IsValid(GetOwner()) && GetOwner()->ActorHasTag("Character"))
	{
		OwnerCharacterRef = Cast<ACharacterBase>(GetOwner());
	}

	for (int newTimerIdx = 0; newTimerIdx < 18; newTimerIdx += 1)
	{
		BuffDebuffTimerHandleList.Add(FTimerHandle());
	}
}

bool ACharacterBuffManager::SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(OwnerCharacterRef)) return false;

	FTimerDelegate timerDelegate;
	FTimerHandle& timerHandle = GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType);

	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();
	AWeaponBase* characterWeaponRef = OwnerCharacterRef->GetWeaponActorRef();

	if ((bIsDebuff && GetDebuffIsActive((ECharacterDebuffType)BuffDebuffType))
		|| (!bIsDebuff && GetBuffIsActive((ECharacterBuffType)BuffDebuffType)))
	{
		const float newTime = GetWorldTimerManager().GetTimerRemaining(timerHandle) + TotalTime;
		GetWorldTimerManager().SetTimer(timerHandle, 1.0f, false, newTime);
		return true;
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
			timerDelegate.BindUFunction(OwnerCharacterRef, FName("TakeDebuffDamage"), Variable, BuffDebuffType, Causer);
			GetWorldTimerManager().SetTimer(BuffDebuffTimerHandleList[DEBUFF_DAMAGE_TIMER_IDX], timerDelegate, 1.0f, true);
			break;
			//----스탯 조정 디버프-------------------
		case ECharacterDebuffType::E_Stun:
			OwnerCharacterRef->StopAttack();
			characterState.CharacterActionState = ECharacterActionType::E_Stun;
			OwnerCharacterRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			break;
		case ECharacterDebuffType::E_Slow:
			OwnerCharacterRef->GetCharacterMovement()->MaxWalkSpeed *= Variable;
			characterStat.CharacterAtkSpeed *= Variable;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->WeaponStat.WeaponAtkSpeedRate *= Variable;
			}
			break;
		case ECharacterDebuffType::E_AttackDown:
			characterStat.CharacterAtkMultiplier *= Variable;
			break;
		case ECharacterDebuffType::E_DefenseDown:
			characterStat.CharacterDefense *= Variable;
			break;
		}
		timerDelegate.BindUFunction(this, FName("ResetStatBuffDebuffState"), bIsDebuff, BuffDebuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 0.1f, false, TotalTime);
		OwnerCharacterRef->UpdateCharacterStat(characterStat);
		OwnerCharacterRef->UpdateCharacterState(characterState);

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
			timerDelegate.BindUFunction(OwnerCharacterRef, FName("TakeHeal"), Variable, true, BuffDebuffType);
			GetWorldTimerManager().SetTimer(BuffDebuffTimerHandleList[HEAL_BUFF_TIMER_IDX], timerDelegate, 1.0f, true);
			break;
			//----스탯 조정 버프-------------------
		case ECharacterBuffType::E_AttackUp:
			characterStat.CharacterAtkMultiplier *= Variable;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->WeaponStat.WeaponDamageRate *= Variable;
			}
			break;
		case ECharacterBuffType::E_DefenseUp:
			characterStat.CharacterDefense *= Variable;
			break;
		case ECharacterBuffType::E_SpeedUp:
			OwnerCharacterRef->GetCharacterMovement()->MaxWalkSpeed *= Variable;
			characterStat.CharacterAtkSpeed *= Variable;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->WeaponStat.WeaponAtkSpeedRate *= Variable;
			}
			break;
		case ECharacterBuffType::E_Invincibility:
			characterStat.bIsInvincibility = true;
			break;
		case ECharacterBuffType::E_InfiniteAmmo:
			if (characterStat.bInfiniteAmmo) return false; //스탯으로써 무한 탄약 능력을 갖고 있다면?
			if (IsValid(characterWeaponRef))
			{
				characterStat.bInfiniteAmmo = true;
				characterWeaponRef->SetInfiniteAmmoMode(true);
			}
			break;
		}
		timerDelegate.BindUFunction(this, FName("ResetStatBuffDebuffState"), bIsDebuff, BuffDebuffType, Variable);
		GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 0.1f, false, TotalTime);
		OwnerCharacterRef->UpdateCharacterStat(characterStat);
		OwnerCharacterRef->UpdateCharacterState(characterState);

		return true;
	}
	return false;
}

void ACharacterBuffManager::ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal)
{
	if (!IsValid(OwnerCharacterRef)) return;

	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();
	AWeaponBase* characterWeaponRef = OwnerCharacterRef->GetWeaponActorRef();

	if (bIsDebuff)
	{
		ResetVal = FMath::Max(0.0f, ResetVal);
		characterState.CharacterDebuffState &= ~(1 << (int)BuffDebuffType);

		switch ((ECharacterDebuffType)BuffDebuffType)
		{
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			GetWorldTimerManager().ClearTimer(BuffDebuffTimerHandleList[DEBUFF_DAMAGE_TIMER_IDX]);
			break;
		case ECharacterDebuffType::E_Slow:
			OwnerCharacterRef->GetCharacterMovement()->MaxWalkSpeed = characterStat.CharacterMoveSpeed;
			characterStat.CharacterAtkSpeed /= ResetVal;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->WeaponStat.WeaponAtkSpeedRate /= ResetVal;
			}
			break;
		case ECharacterDebuffType::E_Stun:
			OwnerCharacterRef->ResetActionState();
			OwnerCharacterRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;
		case ECharacterDebuffType::E_AttackDown:
			characterStat.CharacterAtkMultiplier /= ResetVal;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->WeaponStat.WeaponDamageRate /= ResetVal;
			}
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
			GetWorldTimerManager().ClearTimer(BuffDebuffTimerHandleList[HEAL_BUFF_TIMER_IDX]);
			break;
		case ECharacterBuffType::E_DefenseUp:
			characterStat.CharacterDefense /= ResetVal;
			break;
		case ECharacterBuffType::E_AttackUp:
			characterStat.CharacterAtkMultiplier /= ResetVal;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->WeaponStat.WeaponDamageRate /= ResetVal;
			}
			break;
		case ECharacterBuffType::E_SpeedUp:
			OwnerCharacterRef->GetCharacterMovement()->MaxWalkSpeed /= ResetVal;
			characterStat.CharacterAtkSpeed /= ResetVal;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->WeaponStat.WeaponAtkSpeedRate /= ResetVal;
			}
			break;
		case ECharacterBuffType::E_Invincibility:
			characterStat.bIsInvincibility = false;
			ClearAllBuffDebuffTimer(true);
			break;
		case ECharacterBuffType::E_InfiniteAmmo:
			characterStat.bInfiniteAmmo = false;
			if (IsValid(characterWeaponRef))
			{
				characterWeaponRef->SetInfiniteAmmoMode(false);
			}
			break;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("DEACTIVATE #00"));
	OwnerCharacterRef->UpdateCharacterStat(characterStat);
	OwnerCharacterRef->UpdateCharacterState(characterState);
	ClearBuffDebuffTimer(bIsDebuff, BuffDebuffType);
}

void ACharacterBuffManager::ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType)
{
	if (!IsValid(OwnerCharacterRef)) return;
	BuffEmitterDeactivateDelegate.Broadcast();
	OwnerCharacterRef->AddNewBuffDebuff(bIsDebuff, BuffDebuffType, false);
	GetWorldTimerManager().ClearTimer(GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType));
}

void ACharacterBuffManager::ClearAllBuffDebuffTimer(bool bIsDebuff)
{
	const uint16 startIdx = bIsDebuff ? MAX_BUFF_IDX + 2 : 0;
	const uint16 endIdx = bIsDebuff ? startIdx + MAX_DEBUFF_IDX : MAX_BUFF_IDX + 1;

	UE_LOG(LogTemp, Warning, TEXT("DEACTIVATE #01"));
	BuffEmitterDeactivateDelegate.Broadcast();
	
	for (uint16 timerIdx = startIdx; timerIdx <= endIdx; timerIdx++)
	{
		GetWorldTimerManager().ClearTimer(GetBuffDebuffTimerHandleRef(bIsDebuff, timerIdx));
	}
}

bool ACharacterBuffManager::SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	return SetBuffDebuffTimer(false, (uint8)BuffType, Causer, TotalTime, Variable);
}

bool ACharacterBuffManager::SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	return SetBuffDebuffTimer(true, (uint8)DebuffType, Causer, TotalTime, Variable);
}

bool ACharacterBuffManager::GetDebuffIsActive(ECharacterDebuffType DebuffType)
{
	if (OwnerCharacterRef->GetCharacterState().CharacterDebuffState & (1 << (int)DebuffType)) return true;
	return false;
}

bool ACharacterBuffManager::GetBuffIsActive(ECharacterBuffType BuffType)
{
	if (OwnerCharacterRef->GetCharacterState().CharacterBuffState & (1 << (int)BuffType)) return true;
	return false;
}

float ACharacterBuffManager::GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType)
{
	FTimerHandle& targetBuffTimer = GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType);
	return GetWorldTimerManager().GetTimerRemaining(targetBuffTimer);
}

FTimerHandle& ACharacterBuffManager::GetBuffDebuffTimerHandleRef(bool bIsDebuff, uint8 BuffDebuffType)
{
	int16 targetListIdx = bIsDebuff ? BuffDebuffType + MAX_BUFF_IDX + 1 : BuffDebuffType;
	targetListIdx = BuffDebuffTimerHandleList.IsValidIndex(targetListIdx) ? targetListIdx : 0;
	return BuffDebuffTimerHandleList[targetListIdx];
}