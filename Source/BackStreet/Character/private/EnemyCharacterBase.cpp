// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/EnemyCharacterBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/TileBase.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	/*static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Map/D_StageEnemyRank"));
	if (DataTable.Succeeded())
	{
		EnemyStatDataTable = DataTable.Object;
	}*/
	bUseControllerRotationYaw = false;
	this->Tags.Add("Enemy");
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	TileRef = GamemodeRef->CurrentTile;
	InitEnemyStat();
	
}

void AEnemyCharacterBase::InitEnemyStat()
{
	TArray< FEnemyStatStruct*> DataTable;
	FString ContextString;
	EnemyStatDataTable->GetAllRows(ContextString, DataTable);

	for (FEnemyStatStruct* Row : DataTable)
	{
		if (Row->EnemyID == EnemyID)
		{
			UE_LOG(LogTemp, Log, TEXT("Init Enemy %d"), EnemyID);
			FCharacterStatStruct NewStat;
			NewStat.CharacterMaxHP = Row->CharacterMaxHP;
			NewStat.CharacterAtkMultiplier = Row->CharacterAtkMultiplier;
			NewStat.CharacterAtkSpeed = Row->CharacterAtkSpeed;
			NewStat.CharacterMoveSpeed = Row->CharacterMoveSpeed;
			NewStat.CharacterDefense = Row->CharacterDefense;
			this->UpdateCharacterStat(NewStat);
		}
	}
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

void AEnemyCharacterBase::Turn(float Angle)
{
	FRotator newRotation =  GetActorRotation();
	newRotation.Yaw += Angle;
	SetActorRotation(newRotation);
	
	if (GetVelocity().Length() == 0.0f)
	{
		if (FMath::Abs(Angle) > 0.0f)
		{
			CharacterState.TurnDirection = (FMath::Sign(Angle) == 1 ? 2 : 1);
			return;
		}
	}
	CharacterState.TurnDirection = 0;
}

bool AEnemyCharacterBase::SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime, float Variable)
{
	bool result = Super::SetBuffTimer(bIsDebuff, BuffType, Causer, TotalTime, Variable);
	return result;
}

void AEnemyCharacterBase::ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal)
{
	Super::ResetStatBuffState(bIsDebuff, BuffType, ResetVal);

}
