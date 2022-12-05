// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterBase.h"
#include "../public/WeaponBase.h"
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
	GetCharacterMovement()->GravityScale = 2.5f;
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
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacterBase::Attack);
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
	LaunchCharacter(GetMesh()->GetForwardVector() + FVector( 0.0f, 0.0f, 500.0f ), false, false);
	CharacterState.bIsRolling = true; 

	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]() {
		CharacterState.bIsRolling = false;
	}), 0.5f, false);
}

void AMainCharacterBase::Attack()
{
	Super::Attack();
	if (IsValid(WeaponActor->GetChildActor()))
	{
		AWeaponBase* weaponRef = Cast<AWeaponBase>(WeaponActor->GetChildActor());

		UE_LOG(LogTemp, Warning, TEXT("idx : %d"), weaponRef->GetAttackAnimIndex());
		if (weaponRef->GetAttackAnimIndex() < AttackAnimMontageArray.Num())
		{
			PlayAnimMontage(AttackAnimMontageArray[weaponRef->GetAttackAnimIndex()]);
		}
		weaponRef->Attack();
	}
}

void AMainCharacterBase::StopAttack()
{
	Super::StopAttack();
	if (IsValid(WeaponActor->GetChildActor()))
	{
		AWeaponBase* weaponRef = Cast<AWeaponBase>(WeaponActor->GetChildActor());
		weaponRef->StopAttack();
	}
}
