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
	if (DamageAmount <= 0.0f) return 0.0f;// || !IsValid(DamageCauser)) return 0.0f;

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

float ACharacterBase::TakeDebuffDamage(float DamageAmount, ECharacterDebuffType DebuffType)
{
	//���� ����� �� ��ƼŬ �ý��� �߰� ����

	DebuffRemainingTime[(int)DebuffType] -= 0.25f;
	if (DebuffRemainingTime[(int)DebuffType] <= 0.0f)
	{
		CharacterState.CharacterDebuffState &= ~(1 << (int)DebuffType); //��Ʈ����ũ ���� (���� �ش� ������� �ɷ����� üũ)
		GetWorld()->GetTimerManager().ClearTimer(DebuffTimerHandle[(int)DebuffType]);
	}
	return DamageAmount;
}

void ACharacterBase::SetDebuffTimer(AActor* Causer, float TotalTime, ECharacterDebuffType DebuffType, FTimerHandle& TimerHandle)
{
	switch (DebuffType)
	{
	case ECharacterDebuffType::E_Flame:
		TimerDelegate[(int)DebuffType].BindUFunction(this, FName("TakeDebuffDamage"), 0.1f, DebuffType);
		DebuffRemainingTime[(int)DebuffType] = TotalTime;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate[(int)DebuffType], 0.25f, true);
		break;

	case ECharacterDebuffType::E_Poison:
		break;

	case ECharacterDebuffType::E_Sleep:
		break;

	case ECharacterDebuffType::E_Slow:
		break;
	}
}

void ACharacterBase::SetDebuffTimer(AActor* Causer, float TotalTime, ECharacterDebuffType DebuffType)
{
	if (CharacterState.CharacterDebuffState & (1 << (int)DebuffType)) return; //�̹� Ÿ�̸Ӱ� ������ �ʾҴٸ� return
	CharacterState.CharacterDebuffState |= (1 << (int)DebuffType); //��Ʈ����ũ ���� (���� �ش� ������� �ɷ����� üũ)
	 
	FTimerHandle& timerHandle = DebuffTimerHandle[(int)DebuffType];
	if (!GetWorld()->GetTimerManager().IsTimerActive(timerHandle))
	{
		SetDebuffTimer(Causer, TotalTime, DebuffType, timerHandle);
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

