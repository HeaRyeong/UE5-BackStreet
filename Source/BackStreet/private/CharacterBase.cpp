// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/CharacterBase.h"



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
	if (DamageAmount <= 0.0f || !IsValid(DamageCauser)) return 0.0f;
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

void ACharacterBase::SetDebuffTimer(float TotalTime, float Damage, ECharacterDebuffType DebuffType, AActor* Causer)
{
	if (CharacterState.CharacterDebuffState & (1 << (int)DebuffType))
	{
		DebuffRemainingTime[(int)DebuffType] += TotalTime;
		return;
	}
	CharacterState.CharacterDebuffState |= (1 << (int)DebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)

	FTimerDelegate TimerDelegate;
	FTimerHandle& timerHandle = DebuffTimerHandle[(int)DebuffType];

	if (!GetWorldTimerManager().IsTimerActive(timerHandle))
	{
		DebuffRemainingTime[(int)DebuffType] = TotalTime;
		switch (DebuffType)
		{
		case ECharacterDebuffType::E_Flame:
		case ECharacterDebuffType::E_Poison:
			//SpawnDebuffParticle(E_Flame, TotalTime) //TimerDelegate에 걸어도 됨
			TimerDelegate.BindUFunction(this, FName("TakeDebuffDamage"), Damage, DebuffType, Causer);
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 1.0f, true);
			UE_LOG(LogTemp, Warning, TEXT("FLAME || POISON"));
			break;

		case ECharacterDebuffType::E_Sleep:
			TimerDelegate.BindUFunction(this, FName("ResetNonDamageDebuff"), DebuffType);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
			break;

		case ECharacterDebuffType::E_Slow:
			const float normalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
			TimerDelegate.BindUFunction(this, FName("ResetNonDamageDebuff"), DebuffType, normalWalkSpeed);
			GetCharacterMovement()->MaxWalkSpeed *= 0.5f;
			UE_LOG(LogTemp, Warning, TEXT("SLOW!!"));
			GetWorldTimerManager().SetTimer(timerHandle, TimerDelegate, 0.1f, false, TotalTime);
			break;
		}
	}
}

float ACharacterBase::TakeDebuffDamage(float DamageAmount, ECharacterDebuffType DebuffType, AActor* Causer)
{
	//추후 디버프 당 파티클 시스템 추가 예정
	if (!IsValid(Causer) || DebuffRemainingTime[(int)DebuffType] <= 0.0f) return 0.0f;
		
	DebuffRemainingTime[(int)DebuffType] -= 1.0f;

	TakeDamage(DamageAmount, FDamageEvent(), nullptr, Causer);

	if (DebuffRemainingTime[(int)DebuffType] <= 0.0f)
	{
		CharacterState.CharacterDebuffState &= ~(1 << (int)DebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
		ClearDebuffTimer(DebuffType);
	}
	return DamageAmount;
}

void ACharacterBase::ResetNonDamageDebuff(ECharacterDebuffType DebuffType, float ResetValue)
{
	switch (DebuffType)
	{
	case ECharacterDebuffType::E_Sleep:
		//WakeUp Animation
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		break;
	case ECharacterDebuffType::E_Slow:
		GetCharacterMovement()->MaxWalkSpeed = ResetValue;
		break;
	}
	ClearDebuffTimer(DebuffType);
}

void ACharacterBase::ClearDebuffTimer(ECharacterDebuffType DebuffType)
{
	UE_LOG(LogTemp, Warning, TEXT("ClearTimer!!"));
	DebuffRemainingTime[(int)DebuffType] = 0.0f;
	GetWorldTimerManager().ClearTimer(DebuffTimerHandle[(int)DebuffType]);
}

void ACharacterBase::ClearAllDebuffTimer()
{
	for (int timerIdx = 0; timerIdx < 10; timerIdx++)
	{
		GetWorldTimerManager().ClearTimer(DebuffTimerHandle[timerIdx]);
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

