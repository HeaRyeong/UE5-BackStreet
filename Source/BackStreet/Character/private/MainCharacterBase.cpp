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

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacterBase::Dash);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainCharacterBase::Roll);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacterBase::TryAttack);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacterBase::TryReload);
}

void AMainCharacterBase::MoveForward(float Value)
{
	FVector Direction = FVector(1.0f, 0.0f, 0.0f);// FRotationMatrix(this->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::MoveRight(float Value)
{
	FVector Direction = FVector(0.0f, 1.0f, 0.0f);//FRotationMatrix(this->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::Dash()
{
	if (!IsValid(RollAnimMontage) || !GetIsActionActive(ECharacterActionType::E_Idle)) return;
	
	FVector newDirection(0.0f);
	newDirection.X = GetInputAxisValue(FName("MoveForward"));
	newDirection.Y = GetInputAxisValue(FName("MoveRight"));

	FRotator newRotation = { 0, FMath::Atan2(newDirection.Y, newDirection.X) * 180.0f / 3.141592, 0.0f};
	newRotation.Yaw += 270.0f;

	GetMesh()->SetWorldRotation(newRotation);

	CharacterState.CharacterActionState = ECharacterActionType::E_Roll;
	PlayAnimMontage(RollAnimMontage);

	GetWorld()->GetTimerManager().SetTimer(RollTimerHandle, FTimerDelegate::CreateLambda([&]() {
		ResetActionState();
	}), 0.5f, false);
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

	BlueprintAttackTest();

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

	GetWorld()->GetTimerManager().ClearTimer(RotationFixTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RotationFixTimerHandle, FTimerDelegate::CreateLambda([&]() {
		newRotation = GetCapsuleComponent()->GetComponentRotation();
		newRotation.Yaw += 270.0f;
		GetMesh()->SetWorldRotation(newRotation);
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}), 1.0f, false);
}

bool AMainCharacterBase::SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime, float Variable)
{
	bool result = Super::SetBuffTimer(bIsDebuff, BuffType, Causer, TotalTime, Variable);

	if(result)
	{
		TArray<UNiagaraSystem*>* targetEmitterList = (bIsDebuff ? &DebuffNiagaraEffectList : &BuffNiagaraEffectList);

		UE_LOG(LogTemp, Warning, TEXT("%d %d"), bIsDebuff ? 1 : 0, BuffType);
		if (targetEmitterList->IsValidIndex(BuffType) && (*targetEmitterList)[BuffType] != nullptr)
		{
			BuffNiagaraEmitter->SetRelativeLocation(bIsDebuff ? FVector(0.0f, 0.0f, 125.0f) : FVector(0.0f));
			BuffNiagaraEmitter->Deactivate();
			BuffNiagaraEmitter->SetAsset((*targetEmitterList)[BuffType], false);
			BuffNiagaraEmitter->Activate();
		}
	}
	return result;
}	

void AMainCharacterBase::ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal)
{
	Super::ResetStatBuffState(bIsDebuff, BuffType, ResetVal);
}

void AMainCharacterBase::ClearBuffTimer(bool bIsDebuff, uint8 BuffType)
{
	if (bIsDebuff ? GetDebuffIsActive((ECharacterDebuffType)BuffType)
		: GetBuffIsActive((ECharacterBuffType)BuffType))
	{
		DeactivateBuffNiagara();
	}
	Super::ClearBuffTimer(bIsDebuff, BuffType);
}

void AMainCharacterBase::ClearAllBuffTimer(bool bIsDebuff)
{
	Super::ClearAllBuffTimer(bIsDebuff);
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

	GetWorldTimerManager().ClearTimer(RotationFixTimerHandle);
	GetWorldTimerManager().ClearTimer(RollTimerHandle); 
	GetWorldTimerManager().ClearTimer(AttackLoopTimerHandle);
}
