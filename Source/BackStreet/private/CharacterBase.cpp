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
	
	UpdateCharacterStat();
}

void ACharacterBase::UpdateCharacterStat()
{
	CharacterState.PlayerCurrHP = CharacterState.CharacterMaxHP;
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator
								, AActor* DamageCauser)
{
	if (DamageAmount < 0.0f || !IsValid(DamageCauser)) return 0.0f;

	CharacterState.PlayerCurrHP -= DamageAmount;
	CharacterState.PlayerCurrHP = FMath::Max(0.0f, CharacterState.PlayerCurrHP - DamageAmount);

	return DamageAmount;
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

