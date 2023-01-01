// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/EnemyCharacterBase.h"
#include "../public/BackStreetGameModeBase.h"
#include "Tile.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Character/EnemyCharacter/EnemyRank"));
	if (DataTable.Succeeded())
	{
		EnemyRankDataTable = DataTable.Object;
	}

}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	TileRef = GameModeRef->CurrTile;
	InitEnemyStat();
	
}

void AEnemyCharacterBase::InitEnemyStat()
{
	FStageEnemyRankStruct* StageTableRow = EnemyRankDataTable->FindRow<FStageEnemyRankStruct>(FName(*(FString::FormatAsNumber(GameModeRef->StageLevel))), FString(""));

	if (!TileRef->IsClear)
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



