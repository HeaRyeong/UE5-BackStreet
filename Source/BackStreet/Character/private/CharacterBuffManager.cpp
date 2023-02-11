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
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
	SetActorTickEnabled(false);
}

void ACharacterBuffManager::BeginPlay()
{
	Super::BeginPlay();
	for (int newTimerIdx = 0; newTimerIdx < 18; newTimerIdx += 1)
	{
		BuffDebuffTimerHandleList.Add(FTimerHandle());
	}
}

bool ACharacterBuffManager::SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(OwnerCharacterRef)) return false;

	UE_LOG(LogTemp, Warning, TEXT("BUFF #3"));

	FTimerDelegate timerDelegate;
	FTimerHandle& timerHandle = GetBuffDebuffTimerHandleRef(bIsDebuff, BuffDebuffType);

	FCharacterStateStruct characterState = OwnerCharacterRef->GetCharacterState();
	FCharacterStatStruct characterStat = OwnerCharacterRef->GetCharacterStat();

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
		UE_LOG(LogTemp, Warning, TEXT("BUFF #4"));

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
			break;
		case ECharacterBuffType::E_DefenseUp:
			characterStat.CharacterDefense *= Variable;
			break;
		case ECharacterBuffType::E_SpeedUp:
			OwnerCharacterRef->GetCharacterMovement()->MaxWalkSpeed *= Variable;
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
			break;
		case ECharacterDebuffType::E_Stun:
			OwnerCharacterRef->ResetActionState();
			OwnerCharacterRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
			GetWorldTimerManager().ClearTimer(BuffDebuffTimerHandleList[HEAL_BUFF_TIMER_IDX]);
			break;
		case ECharacterBuffType::E_DefenseUp:
			characterStat.CharacterDefense /= ResetVal;
			break;
		case ECharacterBuffType::E_AttackUp:
			characterStat.CharacterAtkMultiplier /= ResetVal;
			break;
		case ECharacterBuffType::E_SpeedUp:
			OwnerCharacterRef->GetCharacterMovement()->MaxWalkSpeed /= ResetVal;
			characterStat.CharacterAtkSpeed /= ResetVal;
			break;
		case ECharacterBuffType::E_Invincibility:
			characterStat.bIsInvincibility = false;
			ClearAllBuffDebuffTimer(true);
			break;
		case ECharacterBuffType::E_Infinite:
			characterStat.bInfinite = false;
			break;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("DEACTIVATE #00"));
	OwnerCharacterRef->UpdateCharacterStat(characterStat);
	OwnerCharacterRef->UpdateCharacterState(characterState);
}

void ACharacterBuffManager::ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType)
{
	if (!IsValid(OwnerCharacterRef)) return;
	BuffEmitterDeactivateDelegate.Broadcast();
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

void ACharacterBuffManager::InitBuffManager(ACharacterBase* NewOwnerRef)
{
	if (!IsValid(NewOwnerRef)) return;
	SetOwner(NewOwnerRef);
	OwnerCharacterRef = NewOwnerRef;
}

bool ACharacterBuffManager::SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	UE_LOG(LogTemp, Warning, TEXT("BUFF #2"));
	return SetBuffDebuffTimer(false, (uint8)BuffType, Causer, TotalTime, Variable);
}

bool ACharacterBuffManager::SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime, float Variable)
{
	if (!IsValid(Causer) || TotalTime == 0.0f) return false;
	return SetBuffDebuffTimer(true, (uint8)DebuffType, Causer, TotalTime, Variable);
}

bool ACharacterBuffManager::GetDebuffIsActive(ECharacterDebuffType DebuffType)
{
	if (!IsValid(OwnerCharacterRef)) return false;
	if (OwnerCharacterRef->GetCharacterState().CharacterDebuffState & (1 << (int)DebuffType)) return true;
	return false;
}

bool ACharacterBuffManager::GetBuffIsActive(ECharacterBuffType BuffType)
{
	if (!IsValid(OwnerCharacterRef)) return false;
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