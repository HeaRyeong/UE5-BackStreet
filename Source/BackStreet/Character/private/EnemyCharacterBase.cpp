// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/EnemyCharacterBase.h"
#include "../public/CharacterInfoStructBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/Tile.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Character/EnemyCharacter/EnemyRank"));
	if (DataTable.Succeeded())
	{
		EnemyRankDataTable = DataTable.Object;
	}
	this->Tags.Add("Enemy");
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	TileRef = GamemodeRef->CurrTile;
	InitEnemyStat();
	
}

void AEnemyCharacterBase::InitEnemyStat()
{
	if (!IsValid(TileRef)) return;
	FStageEnemyRankStruct* StageTableRow = EnemyRankDataTable->FindRow<FStageEnemyRankStruct>(FName(*(FString::FormatAsNumber(TileRef->StageLevel))), FString(""));

	if (!TileRef->bIsClear)
	{
		// ���� ����
		FCharacterStatStruct NewStat;
		NewStat.CharacterMaxHP = StageTableRow->CharacterMaxHP;
		NewStat.CharacterAtkMultiplier = StageTableRow->CharacterAtkMultiplier;
		NewStat.CharacterAtkSpeed = StageTableRow->CharacterAtkSpeed;
		NewStat.CharacterMoveSpeed = StageTableRow->CharacterMoveSpeed;
		NewStat.CharacterDefense = StageTableRow->CharacterDefense;
		this->UpdateCharacterStat(NewStat);
		// ���� ����Ʈ�� �߰�
		TileRef -> MonsterList.Add(this);
	}
}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("CamEffect"))
	if (IsValid(DamageCauser) && DamageCauser->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("SUCCESS!!"))
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
