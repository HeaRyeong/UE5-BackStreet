// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/CharacterBase.h"


FTimerHandle BuffTimerHandle[2][10];
float BuffRemainingTime[2][10];

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitCharacterState();
}

void ACharacterBase::InitCharacterState()
{
	GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
	CharacterState.CharacterCurrHP = CharacterStat.CharacterMaxHP;
}

void ACharacterBase::UpdateCharacterStat(FCharacterStatStruct NewStat)
{
	CharacterStat = NewStat;
	GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator
								, AActor* DamageCauser)
{
	DamageAmount = DamageAmount - DamageAmount * CharacterStat.CharacterDefense;
	if (DamageAmount <= 0.0f || !IsValid(DamageCauser)) return 0.0f;
	if (CharacterState.bIsInvincibility) return 0.0f;
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterState.CharacterCurrHP -= DamageAmount;
	CharacterState.CharacterCurrHP = FMath::Max(0.0f, CharacterState.CharacterCurrHP - DamageAmount);
	if (CharacterState.CharacterCurrHP == 0.0f)
	{
		Die();
	}
	else if (IsValid(HitAnimMontage))
	{
		PlayAnimMontage(HitAnimMontage);
	}
	return DamageAmount;
}

float ACharacterBase::TakeDebuffDamage(float DamageAmount, uint8 DebuffType, AActor* Causer)
{
	//추후 디버프 당 파티클 시스템 추가 예정
	if (!IsValid(Causer) || BuffRemainingTime[1][(int)DebuffType] <= 0.0f) return 0.0f;

	BuffRemainingTime[1][(int)DebuffType] -= 1.0f;

	TakeDamage(DamageAmount, FDamageEvent(), nullptr, Causer);

	if (BuffRemainingTime[1][(int)DebuffType] <= 0.0f)
	{
		CharacterState.CharacterDebuffState &= ~(1 << (int)DebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
		ClearBuffTimer(1, DebuffType);
	}
	return DamageAmount;
}

void ACharacterBase::TakeHeal(float HealAmount, bool bIsTimerEvent, uint8 BuffType)
{
	CharacterState.CharacterCurrHP += HealAmount;
	CharacterState.CharacterCurrHP = FMath::Min(CharacterStat.CharacterMaxHP
												, CharacterState.CharacterCurrHP);
	if (bIsTimerEvent)
	{
		BuffRemainingTime[0][(int)BuffType] -= 1.0f;
		if (BuffRemainingTime[0][(int)BuffType] <= 0.0f)
		{
			ClearBuffTimer(false, BuffType);
		}
		return;
	}
}

void ACharacterBase::SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime, float Variable)
{
	FTimerDelegate TimerDelegate;
	FTimerHandle& timerHandle = BuffTimerHandle[bIsDebuff][(int)BuffType];
	
	if (CharacterState.CharacterBuffState & (1 << (int)BuffType))
	{
		BuffRemainingTime[bIsDebuff][(int)BuffType] += TotalTime;
		return;
	}
	if (GetWorldTimerManager().IsTimerActive(timerHandle)) return;

	CharacterState.CharacterBuffState |= (1 << (int)BuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
	BuffRemainingTime[bIsDebuff][(int)BuffType] = TotalTime;
	if(bIsDebuff)
	{
		switch ((ECharacterDebuffType)BuffType)
		{
		//----데미지 디버프-------------------
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			//SpawnBuffParticle(E_Flame, TotalTime) //TimerDelegate에 걸어도 됨
			TimerDelegate.BindUFunction(this, FName("TakeDebuffDamage"), Variable, BuffType, Causer);
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 1.0f, true);
			break;

		//----스탯 조정 디버프-------------------
		case ECharacterDebuffType::E_Sleep:
		case ECharacterDebuffType::E_Slow:
		case ECharacterDebuffType::E_AttackDown:
		case ECharacterDebuffType::E_DefenseDown:
			if (BuffType == (uint8)ECharacterDebuffType::E_Sleep)
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			}
			else if (BuffType == (uint8)ECharacterDebuffType::E_Slow)
			{
				GetCharacterMovement()->MaxWalkSpeed *= FMath::Abs(Variable);
			}
			else if (BuffType == (uint8)ECharacterDebuffType::E_AttackDown)
			{
				CharacterState.CharacterCurrAtkMultiplier *= FMath::Abs(Variable);
			}
			else if (BuffType == (uint8)ECharacterDebuffType::E_DefenseDown)
			{
				CharacterState.CharacterCurrDefense *= FMath::Abs(Variable);
			}
			TimerDelegate.BindUFunction(this, FName("ResetStatBuffState"), bIsDebuff, BuffType);
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
			break;
		}
	}
	else
	{
		switch ((ECharacterBuffType)BuffType)
		{
		//----힐 버프-------------------
		case ECharacterBuffType::E_Healing:
			TimerDelegate.BindUFunction(this, FName("TakeHeal"), Variable, true, BuffType);
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 1.0f, true);
			break;

		//----스탯 조정 버프-------------------
		case ECharacterBuffType::E_DefenseUp:
		case ECharacterBuffType::E_AttackUp:
		case ECharacterBuffType::E_Invincibility:
		case ECharacterBuffType::E_SpeedUp:
			if (BuffType == (uint8)ECharacterDebuffType::E_Sleep)
			{
				CharacterState.CharacterCurrDefense *= FMath::Abs(Variable);
			}
			else if (BuffType == (uint8)ECharacterDebuffType::E_Slow)
			{
				CharacterState.CharacterCurrDefense *= FMath::Abs(Variable);
			}
			else if (BuffType == (uint8)ECharacterDebuffType::E_AttackDown)
			{
				CharacterState.CharacterCurrDefense = 100.0f;
			}
			else if (BuffType == (uint8)ECharacterDebuffType::E_DefenseDown)
			{
				CharacterState.CharacterCurrDefense *= FMath::Abs(Variable);
			}
			TimerDelegate.BindUFunction(this, FName("ResetStatBuffState"), bIsDebuff, BuffType);
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
			break;
		}
	}
}

void ACharacterBase::ResetStatBuffState(bool bIsDebuff, uint8 BuffType)
{
	if (bIsDebuff)
	{
		switch ((ECharacterDebuffType)BuffType)
		{
		case ECharacterDebuffType::E_Sleep:
			//WakeUp Animation
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;
		case ECharacterDebuffType::E_Slow:
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
			UE_LOG(LogTemp, Warning, TEXT("ResetSLOW - %d"), CharacterStat.CharacterMoveSpeed);
			break;
		case ECharacterDebuffType::E_AttackDown:
			CharacterState.CharacterCurrAtkMultiplier = CharacterStat.CharacterAtkMultiplier;
			break;
		case ECharacterDebuffType::E_DefenseDown:
			CharacterState.CharacterCurrDefense = CharacterStat.CharacterDefense;
			break;
		}
	}
	else
	{
		switch ((ECharacterBuffType)BuffType)
		{
		case ECharacterBuffType::E_DefenseUp:
			CharacterState.CharacterCurrDefense = CharacterStat.CharacterDefense;
			break;
		case ECharacterBuffType::E_AttackUp:
			CharacterState.CharacterCurrAtkMultiplier = CharacterStat.CharacterAtkMultiplier;
			break;
		case ECharacterBuffType::E_Invincibility:
			CharacterState.bIsInvincibility = false;
			ClearAllBuffTimer(true);
			break;
		case ECharacterBuffType::E_SpeedUp:
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
			break;
		}
	}
	ClearBuffTimer(bIsDebuff, BuffType);
}

void ACharacterBase::ClearBuffTimer(bool bIsDebuff, uint8 BuffType)
{
	UE_LOG(LogTemp, Warning, TEXT("ClearTimer!!"));
	BuffRemainingTime[bIsDebuff][(int)BuffType] = 0.0f;
	GetWorldTimerManager().ClearTimer(BuffTimerHandle[bIsDebuff][(int)BuffType]);
}

void ACharacterBase::ClearAllBuffTimer(bool bIsDebuff)
{
	for (int timerIdx = 0; timerIdx < 10; timerIdx++)
	{
		GetWorldTimerManager().ClearTimer(BuffTimerHandle[bIsDebuff][timerIdx]);
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

