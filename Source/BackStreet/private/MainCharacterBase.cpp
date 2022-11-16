// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterBase.h"
#include "../public/ProjectileBase.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

// Sets default values
AMainCharacterBase::AMainCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 1400.0f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->SetRelativeRotation({ -60.0f, 0.0f, 0.0f });

	FollowingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FOLLOWING_CAMERA"));
	FollowingCamera->SetupAttachment(CameraBoom);
	FollowingCamera->bAutoActivate = false;

	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->RotationRate = { 0.0f, 0.0f, 750.0f };
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

// Called when the game starts or when spawned
void AMainCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacterBase::MoveRight);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacterBase::Dash);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainCharacterBase::Roll);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacterBase::Fire);
}

void AMainCharacterBase::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(this->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(this->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::Dash()
{
	if (CharacterState.bIsRolling || !IsValid(RollAnimMontage)) return;
	
	PlayAnimMontage(RollAnimMontage);
	GetCharacterMovement()->MaxWalkSpeed += 150.0f;
	CharacterState.bIsRolling = true; 

	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]() {
		GetCharacterMovement()->MaxWalkSpeed -= 150.0f;
		CharacterState.bIsRolling = false;
	}), 0.75f, false);
}

void AMainCharacterBase::Fire()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Projectile의 소유자는 Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f + GetActorRightVector() * 25.0f;
	FRotator SpawnRotation = GetActorRotation();
	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };

	AProjectileBase * Projectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));
	if (Projectile)
	{
		Projectile->ActivateProjectileMovement();
		Projectile->SetSpawnInstigator(GetController());
	}
}
