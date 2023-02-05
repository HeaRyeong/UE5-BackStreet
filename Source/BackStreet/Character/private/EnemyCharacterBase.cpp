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
	AutoPossessAI = EAutoPossessAI::Spawned;

	this->Tags.Add("Enemy");
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	TileRef = GamemodeRef->CurrentTile;

	SetDefaultWeapon();
	SetDefaultStat();
	InitFloatingHpWidget();	
}

void AEnemyCharacterBase::InitEnemyStat()
{

	GamemodeRef->UpdateCharacterStatWithID(this, EnemyID);
}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsValid(DamageCauser) && DamageCauser->ActorHasTag("Player"))
	{
		GamemodeRef->PlayCameraShakeEffect(ECameraShakeType::E_Attack, DamageCauser->GetActorLocation());
	}
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
	CharacterStat.bInfiniteAmmo = true;
	CharacterStat.bInfiniteDurability = true;
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

bool AEnemyCharacterBase::SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime, float Variable)
{
	bool result = Super::SetBuffDebuffTimer(bIsDebuff, BuffDebuffType, Causer, TotalTime, Variable);
	return result;
}

void AEnemyCharacterBase::ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal)
{
	Super::ResetStatBuffDebuffState(bIsDebuff, BuffDebuffType, ResetVal);

}
