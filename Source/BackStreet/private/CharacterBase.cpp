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

void ACharacterBase::UpdateCharacterStat(FPlayerStatStruct NewStat)
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

float ACharacterBase::TakeDebuffDamage(float DamageAmount)
{
	UGameplayStatics::ApplyDamage(this, DamageAmount, GetController(), this, nullptr);
	UE_LOG(LogTemp, Warning, TEXT("TEMP"));
	return DamageAmount;
}

void ACharacterBase::SetDebuffTimer(float Rate, AActor* Causer, FTimerHandle TimerHandle)
{
	FTimerHandle* timerHandle = nullptr;
	for (int timerIdx = 0; timerIdx < 3; timerIdx++)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(DebuffTimerHandle[timerIdx]))
		{
			timerHandle = &DebuffTimerHandle[timerIdx];
			break;
		}
	}
	if (timerHandle)	
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
			TakeDebuffDamage(0.1f); //추후 디버프 종류에 따른 구분 추가 예정 (Damage Type, Debuff Type)
		}), Rate, true);
	}
}

void ACharacterBase::SetDebuffTimer(float Rate, AActor* Causer)
{
	SetDebuffTimer(Rate, Causer, DebuffTimerHandle[0]);
}

void ACharacterBase::ClearDebuffTimer()
{
	for (int timerIdx = 0; timerIdx < 3; timerIdx++)
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

