// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterBase.h"
#include "../public/MainCharacterController.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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

	BuffNiagaraEmitter = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BUFF_EFFECT"));
	BuffNiagaraEmitter->SetupAttachment(GetMesh());
	BuffNiagaraEmitter->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));
	BuffNiagaraEmitter->bAutoActivate = false;

	DirectionNiagaraEmitter = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DIRECTION_EFFECT"));
	DirectionNiagaraEmitter->SetupAttachment(GetMesh());
	DirectionNiagaraEmitter->SetRelativeRotation({ 0.0f, 90.0f, 0.0f });


	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->RotationRate = { 0.0f, 0.0f, 750.0f };
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 2.5f;

	this->Tags.Add("Player");
}

// Called when the game starts or when spawned
void AMainCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerRef = Cast<AMainCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
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
	
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainCharacterBase::Roll);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacterBase::TryAttack);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacterBase::TryReload);
}

void AMainCharacterBase::MoveForward(float Value)
{
	FVector Direction = FVector(1.0f, 0.0f, 0.0f);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::MoveRight(float Value)
{
	FVector Direction = FVector(0.0f, 1.0f, 0.0f);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::Roll()
{
	if (!IsValid(RollAnimMontage) || !GetIsActionActive(ECharacterActionType::E_Idle)) return;
	
	FVector newDirection(0.0f);
	newDirection.X = GetInputAxisValue(FName("MoveForward"));
	newDirection.Y = GetInputAxisValue(FName("MoveRight"));

	FRotator newRotation = { 0, FMath::Atan2(newDirection.Y, newDirection.X) * 180.0f / 3.141592, 0.0f};
	newRotation.Yaw += 270.0f;

	GetMesh()->SetWorldRotation(newRotation);
	GetWorld()->GetTimerManager().ClearTimer(RotationResetTimerHandle); //Roll 도중에 Rotation이 바뀌는 현상 방지

	CharacterState.CharacterActionState = ECharacterActionType::E_Roll;
	PlayAnimMontage(RollAnimMontage, CharacterStat.CharacterMoveSpeed / 450.0f);
}

void AMainCharacterBase::TryReload()
{
	Super::TryReload();
}

float AMainCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (damageAmount > 0.0f && DamageCauser->ActorHasTag("Enemy"))
	{
		GamemodeRef->PlayCameraShakeEffect(ECameraShakeType::E_Hit, GetActorLocation());
	}

	return damageAmount;
}

void AMainCharacterBase::TryAttack()
{
	if (!PlayerControllerRef->GetActionKeyIsDown("Attack")) return; 
	if (CharacterState.CharacterActionState != ECharacterActionType::E_Attack
		&& CharacterState.CharacterActionState != ECharacterActionType::E_Idle) return;

	//공격을 하고, 커서 위치로 Rotation을 조정
	Super::TryAttack();
	RotateToCursor();

	//Pressed 상태를 0.2s 뒤에 체크해서 계속 눌려있다면 Attack 반복
	GetWorldTimerManager().ClearTimer(AttackLoopTimerHandle);
	GetWorldTimerManager().SetTimer(AttackLoopTimerHandle, this
						, &AMainCharacterBase::TryAttack, 1.0f, false, 0.2f);
}

void AMainCharacterBase::Attack()
{
	Super::Attack();
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

void AMainCharacterBase::RotateToCursor()
{
	if (CharacterState.CharacterActionState != ECharacterActionType::E_Idle
		&& CharacterState.CharacterActionState != ECharacterActionType::E_Attack) return;

	FRotator newRotation = PlayerControllerRef->GetAimingRotation();
	if (newRotation != FRotator())
	{
		newRotation.Pitch = newRotation.Roll = 0.0f;
		GetMesh()->SetWorldRotation(newRotation);
	}
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetWorld()->GetTimerManager().ClearTimer(RotationResetTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RotationResetTimerHandle, FTimerDelegate::CreateLambda([&]() {
		ResetRotationToMovement();
	}), 1.0f, false);
}

void AMainCharacterBase::ResetRotationToMovement()
{
	if (CharacterState.CharacterActionState == ECharacterActionType::E_Roll) return;
	FRotator newRotation = GetCapsuleComponent()->GetComponentRotation();
	newRotation.Yaw += 270.0f;
	GetMesh()->SetWorldRotation(newRotation);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

bool AMainCharacterBase::SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Variable)
{
	bool result = Super::SetBuffDebuffTimer(bIsDebuff, BuffDebuffType, Causer, TotalTime, Variable);

	if(result)
	{
		TArray<UNiagaraSystem*>* targetEmitterList = (bIsDebuff ? &DebuffNiagaraEffectList : &BuffNiagaraEffectList);
		
		if (targetEmitterList->IsValidIndex(BuffDebuffType) && (*targetEmitterList)[BuffDebuffType] != nullptr)
		{
			BuffNiagaraEmitter->SetRelativeLocation(bIsDebuff ? FVector(0.0f, 0.0f, 125.0f) : FVector(0.0f));
			BuffNiagaraEmitter->Deactivate();
			BuffNiagaraEmitter->SetAsset((*targetEmitterList)[BuffDebuffType], false);
			BuffNiagaraEmitter->Activate();
		}
	}
	return result;
}	

void AMainCharacterBase::ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal)
{
	Super::ResetStatBuffDebuffState(bIsDebuff, BuffDebuffType, ResetVal);
}

void AMainCharacterBase::ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType)
{
	if (bIsDebuff ? GetDebuffIsActive((ECharacterDebuffType)BuffDebuffType)
		: GetBuffIsActive((ECharacterBuffType)BuffDebuffType))
	{
		DeactivateBuffNiagara();
	}
	Super::ClearBuffDebuffTimer(bIsDebuff, BuffDebuffType);
}

void AMainCharacterBase::ClearAllBuffDebuffTimer(bool bIsDebuff)
{
	Super::ClearAllBuffDebuffTimer(bIsDebuff);
	DeactivateBuffNiagara();
}

void AMainCharacterBase::DeactivateBuffNiagara()
{
	BuffNiagaraEmitter->SetAsset(nullptr, false);
	BuffNiagaraEmitter->Deactivate();
}

void AMainCharacterBase::ClearAllTimerHandle()
{
	Super::ClearAllTimerHandle();

	GetWorldTimerManager().ClearTimer(RotationResetTimerHandle);
	GetWorldTimerManager().ClearTimer(RollTimerHandle); 
	GetWorldTimerManager().ClearTimer(AttackLoopTimerHandle);
}
