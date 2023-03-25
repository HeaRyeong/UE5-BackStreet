// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Character.h"
#include "AbilityManagerBase.generated.h"

UCLASS()
class BACKSTREET_API AAbilityManagerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAbilityManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
