// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/AbilityManagerBase.h"

// Sets default values
AAbilityManagerBase::AAbilityManagerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAbilityManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbilityManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAbilityManagerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

