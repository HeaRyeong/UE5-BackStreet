// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/EnemyCharacterBase.h"
#include "../../AISystem/public/AIControllerBase.h"
#include "../public/CharacterInfoStruct.h"
#include "../../Item/public/WeaponInventoryBase.h"
#include "../../StageSystem/public/StageInfoStruct.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Components/WidgetComponent.h"
#include "../../StageSystem/public/TileBase.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	FloatingHpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("FLOATING_HP_BAR"));
	FloatingHpBar->SetupAttachment(GetCapsuleComponent());
	FloatingHpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));
	FloatingHpBar->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));
	FloatingHpBar->SetDrawSize({ 80.0f, 10.0f });

	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	this->Tags.Add("Enemy");
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	TileRef = GamemodeRef->CurrentTile;

	SetDefaultWeapon();
	SetDefaultStat();
	InitFloatingHpWidget();	
	InitEnemyStat();
}

void AEnemyCharacterBase::InitEnemyStat()
{
	GamemodeRef->UpdateCharacterStatWithID(this, EnemyID);
	CharacterState.CharacterCurrHP = CharacterStat.CharacterMaxHP;
	GetCharacterMovement()->MaxWalkSpeed = CharacterStat.CharacterMoveSpeed;
}

bool AEnemyCharacterBase::AddNewBuffDebuff(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Value)
{
	return Super::AddNewBuffDebuff(bIsDebuff, BuffDebuffType, Causer, TotalTime, Value);
}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!IsValid(DamageCauser) || !DamageCauser->ActorHasTag("Player") || damageAmount <= 0.0f) return 0.0f;

	GamemodeRef->PlayCameraShakeEffect(ECameraShakeType::E_Attack, DamageCauser->GetActorLocation()); 
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitImpactSound, GetActorLocation());
	EnemyDamageDelegate.ExecuteIfBound(DamageCauser);

	return damageAmount;
}

void AEnemyCharacterBase::TryAttack()
{
	Super::TryAttack();
}

void AEnemyCharacterBase::Attack()
{
	Super::Attack();
}

void AEnemyCharacterBase::StopAttack()
{
	Super::StopAttack();
}

void AEnemyCharacterBase::Die()
{
	Super::Die();

	EnemyDeathDelegate.ExecuteIfBound(this);
	Controller->Destroy();
}

void AEnemyCharacterBase::SetDefaultWeapon()
{
	if (IsValid(GetInventoryRef()))
	{
		GetInventoryRef()->AddWeapon(DefaultWeaponID);
	}
}

void AEnemyCharacterBase::SetDefaultStat()
{
	CharacterStat.bInfinite = true;
	CharacterStat.bInfinite = true;
}

void AEnemyCharacterBase::Turn(float Angle)
{
	if (FMath::Abs(Angle) == 0.0f)
	{
		CharacterState.TurnDirection = 0;
		return;
	}

	FRotator newRotation =  GetActorRotation();
	newRotation.Yaw += Angle;
	SetActorRotation(newRotation);
	
	if (GetVelocity().Length() == 0.0f)
	{
		CharacterState.TurnDirection = (FMath::Sign(Angle) == 1 ? 2 : 1);
		return;
	}
	CharacterState.TurnDirection = 0;
}

float AEnemyCharacterBase::PlayPreChaseAnimation()
{
	if (PreChaseAnimMontage == nullptr) return 0.0f;

	return PlayAnimMontage(PreChaseAnimMontage);
}
