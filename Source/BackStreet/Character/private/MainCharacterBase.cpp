// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterBase.h"
#include "../public/MainCharacterController.h"
#include "../public/CharacterBuffManager.h"
#include "../../Item/public/WeaponBase.h"
#include "../../Item/public/ItemBase.h"
#include "../../Item/public/ItemBoxBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Components/AudioComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#define MAX_CAMERA_BOOM_LENGTH 1450.0f
#define MIN_CAMERA_BOOM_LENGTH 250.0f

// Sets default values
AMainCharacterBase::AMainCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.1f);

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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SOUND"));


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

	InitDynamicMeshMaterial(NormalMaterial);
}

// Called every frame
void AMainCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWallThroughEffect();
}

// Called to bind functionality to input
void AMainCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("ZoomIn", this, &AMainCharacterBase::ZoomIn);
	
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainCharacterBase::Roll);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacterBase::TryAttack);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacterBase::TryReload);

	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AMainCharacterBase::SwitchToNextWeapon);
	PlayerInputComponent->BindAction("PickItem", IE_Pressed, this, &AMainCharacterBase::TryPickItem);
	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, this, &AMainCharacterBase::DropWeapon);
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

	// 사운드
	if (RollSound->IsValidLowLevelFast())
	{
		UGameplayStatics::PlaySoundAtLocation(this, RollSound, GetActorLocation());
	}

	GetMesh()->SetWorldRotation(newRotation);
	GetWorld()->GetTimerManager().ClearTimer(RotationResetTimerHandle); //Roll 도중에 Rotation이 바뀌는 현상 방지

	CharacterState.CharacterActionState = ECharacterActionType::E_Roll;
	PlayAnimMontage(RollAnimMontage, CharacterStat.CharacterMoveSpeed / 450.0f);
}

void AMainCharacterBase::ZoomIn(float Value)
{
	if (Value == 0.0f) return;
	float newLength = CameraBoom->TargetArmLength;
	newLength = newLength + Value * 25.0f;
	newLength = newLength < MIN_CAMERA_BOOM_LENGTH ? MIN_CAMERA_BOOM_LENGTH : newLength;
	newLength = newLength > MAX_CAMERA_BOOM_LENGTH ? MAX_CAMERA_BOOM_LENGTH : newLength;
	CameraBoom->TargetArmLength = newLength;
}

void AMainCharacterBase::TryPickItem()
{
	TArray<AActor*> nearItemList = GetNearItemList();
	
	UE_LOG(LogTemp, Warning, TEXT("Open #1"));

	if (nearItemList.Num())
	{
		AActor* targetItem = nearItemList[0];
		
		UE_LOG(LogTemp, Warning, TEXT("Open #2"));

		if (targetItem->ActorHasTag("Item"))
		{
			Cast<AItemBase>(targetItem)->OnPlayerBeginPickUp.ExecuteIfBound(this);
			UE_LOG(LogTemp, Warning, TEXT("Pick #3"));
		}
		else if(targetItem->ActorHasTag("ItemBox"))
		{
			Cast<AItemBoxBase>(targetItem)->OnPlayerOpenBegin.ExecuteIfBound(this);
			UE_LOG(LogTemp, Warning, TEXT("Open #2.5"));
		}
	}
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

		SetFacialDamageEffect(true);

		GetWorld()->GetTimerManager().ClearTimer(FacialEffectResetTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(FacialEffectResetTimerHandle, FTimerDelegate::CreateLambda([&]() {
			SetFacialDamageEffect(false);
		}), 1.0f, false, 1.0f);
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
	if (IsValid(GetWeaponActorRef()))
	{
		GetWeaponActorRef()->StopAttack();
	}
}

void AMainCharacterBase::Die()
{
	Super::Die();
	if (IsValid(GamemodeRef))
	{
		GamemodeRef->FinishChapterDelegate.Broadcast(true);
	}
	ClearAllTimerHandle();
	UE_LOG(LogTemp, Warning, TEXT("DIE DELEGATE"));
}

void AMainCharacterBase::RotateToCursor()
{
	if (CharacterState.CharacterActionState != ECharacterActionType::E_Idle
		&& CharacterState.CharacterActionState != ECharacterActionType::E_Attack) return;

	FRotator newRotation = PlayerControllerRef->GetRotationToCursor();
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

TArray<AActor*> AMainCharacterBase::GetNearItemList()
{
	TArray<AActor*> totalItemList;
	TArray<UClass*> targetClassList = {AItemBase::StaticClass(), AItemBoxBase::StaticClass()};
	TEnumAsByte<EObjectTypeQuery> itemObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	FVector overlapBeginPos = GetActorLocation() + GetMesh()->GetForwardVector() * 70.0f + GetMesh()->GetUpVector() * -45.0f;
	
	for (float sphereRadius = 0.2f; sphereRadius < 1.5f; sphereRadius += 0.2f)
	{
		TArray<AActor*> tempItemList; 
		bool result = false;

		for (auto& targetClass : targetClassList)
		{
			result = (result || UKismetSystemLibrary::SphereOverlapActors(GetWorld(), overlapBeginPos, sphereRadius
														, { itemObjectType }, targetClass, {}, tempItemList));

			for (auto& actorRef : tempItemList)
				totalItemList.Add(actorRef);
		}

		if (result) break; 
		else {
			UE_LOG(LogTemp, Warning, TEXT("Pick #0 - Nothing"));
		}
	}
	return totalItemList;
}

void AMainCharacterBase::ResetRotationToMovement()
{
	if (CharacterState.CharacterActionState == ECharacterActionType::E_Roll) return;
	FRotator newRotation = GetCapsuleComponent()->GetComponentRotation();
	newRotation.Yaw += 270.0f;
	GetMesh()->SetWorldRotation(newRotation);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainCharacterBase::SwitchToNextWeapon()
{
	Super::SwitchToNextWeapon();
}

void AMainCharacterBase::DropWeapon()
{
	Super::DropWeapon();
}

bool AMainCharacterBase::AddNewBuffDebuff(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Value)
{
	if (!Super::AddNewBuffDebuff(bIsDebuff, BuffDebuffType, Causer, TotalTime, Value)) return false;

	if (DebuffSound && BuffSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, bIsDebuff ? DebuffSound : BuffSound, GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("BUFF / DEBUFF ACTIVATED"));
	}
	ActivateBuffNiagara(bIsDebuff, BuffDebuffType);

	GetWorld()->GetTimerManager().ClearTimer(BuffEffectResetTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(BuffEffectResetTimerHandle, FTimerDelegate::CreateLambda([&]() {
		DeactivateBuffEffect();
	}), TotalTime, false);

	return true;
}

void AMainCharacterBase::ActivateBuffNiagara(bool bIsDebuff, uint8 BuffDebuffType)
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

void AMainCharacterBase::DeactivateBuffEffect()
{
	BuffNiagaraEmitter->SetAsset(nullptr, false);
	BuffNiagaraEmitter->Deactivate(); 
}

void AMainCharacterBase::UpdateWallThroughEffect()
{
	FHitResult hitResult;
	const FVector& traceBeginPos = FollowingCamera->GetComponentLocation();
	const FVector& traceEndPos = GetMesh()->GetComponentLocation();
	
	GetWorld()->LineTraceSingleByChannel(hitResult, traceBeginPos, traceEndPos, ECollisionChannel::ECC_Camera);

	if (hitResult.bBlockingHit && IsValid(hitResult.GetActor()))
	{
		if(!hitResult.GetActor()->ActorHasTag("Player") && !bIsWallThroughEffectActivated)
		{
			InitDynamicMeshMaterial(WallThroughMaterial);
			bIsWallThroughEffectActivated = true;
		}
		else if(hitResult.GetActor()->ActorHasTag("Player") && bIsWallThroughEffectActivated)
		{
			InitDynamicMeshMaterial(NormalMaterial);
			bIsWallThroughEffectActivated = false;
		}
	}
}

void AMainCharacterBase::SetFacialDamageEffect(bool NewState)
{
	UMaterialInstanceDynamic* currMaterial = CurrentDynamicMaterial;
	
	if (currMaterial != nullptr && EmotionTextureList.Num() >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("FacialEffect"));
		currMaterial->SetTextureParameterValue(FName("BaseTexture"), EmotionTextureList[(uint8)(NewState ? EEmotionType::E_Angry : EEmotionType::E_Idle)]);
		currMaterial->SetScalarParameterValue(FName("bIsDamaged"), (float)NewState);
	}
}

void AMainCharacterBase::ClearAllTimerHandle()
{
	Super::ClearAllTimerHandle();

	GetWorldTimerManager().ClearTimer(BuffEffectResetTimerHandle);
	GetWorldTimerManager().ClearTimer(FacialEffectResetTimerHandle);
	GetWorldTimerManager().ClearTimer(RotationResetTimerHandle);
	GetWorldTimerManager().ClearTimer(RollTimerHandle); 
	GetWorldTimerManager().ClearTimer(AttackLoopTimerHandle);
}
