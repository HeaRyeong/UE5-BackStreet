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

float ACharacterBase::TakeDebuffDamage(float DamageAmount, ECharacterDebuffType DebuffType, AActor* Causer)
{
	//추후 디버프 당 파티클 시스템 추가 예정
	DebuffRemainingTime[(int)DebuffType] -= 0.25f;
	UE_LOG(LogTemp, Warning, TEXT("%lf"), DebuffRemainingTime[(int)DebuffType]);

	if (IsValid(Causer))
	{
		TakeDamage(DamageAmount, FDamageEvent(), nullptr, Causer);
	}
	else
	{
		TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);
	}

	if (DebuffRemainingTime[(int)DebuffType] <= 0.0f)
	{
		CharacterState.CharacterDebuffState &= ~(1 << (int)DebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)
		GetWorld()->GetTimerManager().ClearTimer(DebuffTimerHandle[(int)DebuffType]);
	}
	return DamageAmount;
}

void ACharacterBase::SetDebuffTimer(float TotalTime, ECharacterDebuffType DebuffType, AActor* Causer)
{
	if (CharacterState.CharacterDebuffState & (1 << (int)DebuffType)) return; //이미 타이머가 끝나지 않았다면 return
	CharacterState.CharacterDebuffState |= (1 << (int)DebuffType); //비트마스크 연산 (현재 해당 디버프에 걸렸음을 체크)

	FTimerDelegate TimerDelegate;
	FTimerHandle timerHandle = DebuffTimerHandle[(int)DebuffType];

	if (!GetWorld()->GetTimerManager().IsTimerActive(timerHandle))
	{
		switch (DebuffType)
		{
		case ECharacterDebuffType::E_Flame:
			TimerDelegate.BindUFunction(this, FName("TakeDebuffDamage"), 0.25f, DebuffType, Causer);
			DebuffRemainingTime[(int)DebuffType] = TotalTime;
			GetWorld()->GetTimerManager().SetTimer(timerHandle, TimerDelegate, 0.25f, true);
			break;
		case ECharacterDebuffType::E_Poison:
			break;

		case ECharacterDebuffType::E_Sleep:
			break;

		case ECharacterDebuffType::E_Slow:
			break;
		}
	}
}

void ACharacterBase::ClearDebuffTimer()
{
	for (int timerIdx = 0; timerIdx < 10; timerIdx++)
	{
		GetWorld()->GetTimerManager().ClearTimer(DebuffTimerHandle[timerIdx]);
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

