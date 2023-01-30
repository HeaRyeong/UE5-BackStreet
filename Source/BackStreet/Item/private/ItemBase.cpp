// Fill out your copyright notice in the Description page of Project Settings.
#include "../public/ItemBase.h"
#include "../public/ItemInfoStruct.h"
#include "../public/ItemDataAssetInfo.h"
#include "../public/WeaponBase.h"
#include "../public/WeaponInfoStruct.h"
#include "../../StageSystem/public/MissionBase.h"
#include "../../StageSystem/public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Character/public/CharacterBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	NiagaraCompo = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	OverlapVolume->SetupAttachment(RootComponent);
	NiagaraCompo->SetupAttachment(RootComponent);
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AItemBase::OverlapBegins);

}


void AItemBase::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		FBuffItemInfoStruct Stat;

		UE_LOG(LogTemp, Log, TEXT("Get Item %d"), Type);
		switch (Type)
		{
		case EItemCategoryInfo::E_None:
			break;
		case EItemCategoryInfo::E_Weapon:
			break;
		case EItemCategoryInfo::E_Bullet:
			break;
		case EItemCategoryInfo::E_Buff:
			UE_LOG(LogTemp, Log, TEXT("E_Buff case %d" ), Stat.Type);
			Stat = DA->BuffStat;
			MyCharacter->SetBuffTimer(Stat.Type, this, Stat.Time, Stat.Time);
			Destroy();
			break;
		case EItemCategoryInfo::E_DeBuff:
			break;
		case EItemCategoryInfo::E_StatUp:
			break;
		case EItemCategoryInfo::E_Mission:
			TileRef->MissionInfo->ItemList.Remove(this);
			TileRef->MissionInfo->ClearCheck();
			Destroy();
			break;
		default:
			break;
		}
	}

}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	MyCharacter = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}


void AItemBase::InitItem(EItemCategoryInfo SetType)
{
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	TileRef = GameModeRef->CurrTile;
	AssetManagerRef = GameModeRef->AssetDataManager;

	Type = SetType;

	if (SetType == EItemCategoryInfo::E_Mission)
	{
		TileRef->MissionInfo->ItemList.AddUnique(this);
	}

}