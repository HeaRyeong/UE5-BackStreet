// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "CharacterBase.h"
#include "GameFramework/Character.h"
#include "MainCharacterBase.generated.h"

UCLASS()
class BACKSTREET_API AMainCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void Attack();

	UFUNCTION()
		void Dash();

	UFUNCTION(BlueprintImplementableEvent)
		void Roll();


public:
	//플레이어 메인 카메라 붐
	UPROPERTY(VisibleDefaultsOnly)
		USpringArmComponent* CameraBoom;
	
	//플레이어의 메인 카메라
	UPROPERTY(VisibleDefaultsOnly)
		UCameraComponent* FollowingCamera;
};
