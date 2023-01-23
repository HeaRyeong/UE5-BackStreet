// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/EnemyCharacterBase.h"
#include "../public/CharacterInfoStructBase.h"
#include "../../StageSystem/public/StageInfoStructBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/TileBase.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Map/D_StageEnemyRank"));
	if (DataTable.Succeeded())
	{
		EnemyRankDataTable = DataTable.Object;
	}
	this->Tags.Add("Enemy");
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacterBase::InitEnemyStat()
{
	if (!IsValid(TileRef)) return;
	FStageEnemyRankStruct* StageTableRow = EnemyRankDataTable->FindRow<FStageEnemyRankStruct>(FName(*(FString::FormatAsNumber(TileRef->StageLevel))), FString(""));

	if (!TileRef->bIsClear)
	{
		// 스탯 설정
		FCharacterStatStruct NewStat;
		NewStat.CharacterMaxHP = StageTableRow->CharacterMaxHP;
		NewStat.CharacterAtkMultiplier = StageTableRow->CharacterAtkMultiplier;
		NewStat.CharacterAtkSpeed = StageTableRow->CharacterAtkSpeed;
		NewStat.CharacterMoveSpeed = StageTableRow->CharacterMoveSpeed;
		NewStat.CharacterDefense = StageTableRow->CharacterDefense;
		this->UpdateCharacterStat(NewStat);
		// 몬스터 리스트에 추가
		TileRef -> MonsterList.Add(this);
	}
}

float AEnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
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
