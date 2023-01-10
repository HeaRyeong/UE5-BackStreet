// Fill out your copyright notice in the Description page of Project Settings.
#include "../public/Item.h"
#include "../public/ItemInfoStruct.h"
#include "../public/WeaponStatStructBase.h"
#include "../public/WeaponBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/Tile.h"
#include "../../Character/public/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	OverlapVolume->SetupAttachment(RootComponent);
	Mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UDataTable> BuffItenData(TEXT("/Game/Item/BuffItem"));
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponItenData(TEXT("/Game/Weapon/WeaponItem"));
	if (BuffItenData.Succeeded())
	{
		BuffItemTable = BuffItenData.Object;
	}
	if (WeaponItenData.Succeeded())
	{
		WeaponItemTable = WeaponItenData.Object;
	}

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AItem::OverlapBegins);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	TileRef = GameModeRef->CurrTile;

	if (IsValid(TileRef) && TileRef->bIsClear)
		Destroy();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AItem::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
	{
		uint8 ItemType = FMath::RandRange(0, 1);

		if (ItemType == 0)
		{
			uint8 BuffType = FMath::RandRange(0, 6);

			FBuffItemInfoStruct* BuffItemTableRow = BuffItemTable->FindRow<FBuffItemInfoStruct>(FName(*(FString::FormatAsNumber(BuffType))), FString(""));

			ACharacterBase* player = Cast<ACharacterBase>(OtherActor);
			player->SetBuffTimer(false, BuffType, this, BuffItemTableRow->Time, BuffItemTableRow->Value);

			UE_LOG(LogTemp, Log, TEXT("Get Buff %d"), BuffType);
			Destroy();
		}
		else
		{
			uint8 WeaponType = FMath::RandRange(0, 1);

			FWeaponItemInfoStruct* WeaponItemTableRow = WeaponItemTable->FindRow<FWeaponItemInfoStruct>(FName(*(FString::FormatAsNumber(WeaponType))), FString(""));
			FWeaponStatStruct NewStat;

			NewStat.bCanMeleeAtk = WeaponItemTableRow->bCanMeleeAtk;
			NewStat.WeaponAtkSpeedRate = WeaponItemTableRow->WeaponAtkSpeedRate;
			NewStat.WeaponDamage = WeaponItemTableRow->WeaponDamage;
			NewStat.DebuffType = WeaponItemTableRow->DebuffType;
			NewStat.bHasProjectile = WeaponItemTableRow->bHasProjectile;
			NewStat.bInfiniteMagazine = WeaponItemTableRow->bInfiniteMagazine;
			NewStat.MaxAmmoPerMagazine = WeaponItemTableRow->MaxAmmoPerMagazine;
			NewStat.LoadingDelayTime = WeaponItemTableRow->LoadingDelayTime;

			FProjectileStatStruct NewProjectStat;

			NewProjectStat.ProjectileSpeed = WeaponItemTableRow->ProjectileSpeed;
			NewProjectStat.ProjectileDamage = WeaponItemTableRow->ProjectileDamage;
			NewProjectStat.GravityScale = WeaponItemTableRow->GravityScale;
			NewProjectStat.bIsHoming = WeaponItemTableRow->bIsHoming;
			NewProjectStat.DebuffType = WeaponItemTableRow->ProjectDebuffType;

			NewStat.ProjectileStat = NewProjectStat;

			ACharacterBase* player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			player->GetWeaponActorRef()->InitWeaponStat(NewStat);
			UE_LOG(LogTemp, Log, TEXT("Get Weapon %d"), WeaponType);
			Destroy();
		}

	
	}

}