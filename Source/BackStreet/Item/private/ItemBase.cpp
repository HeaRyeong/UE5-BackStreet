// Fill out your copyright notice in the Description page of Project Settings.
#include "../public/ItemBase.h"
#include "../public/WeaponBase.h"
#include "../../StageSystem/public/MissionBase.h"
#include "../../StageSystem/public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/ChapterManagerBase.h"
#include "../../StageSystem/public/ALevelScriptInGame.h"
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
	//GameModeRef = Cast<ABackStreetGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
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
			UE_LOG(LogTemp, Log, TEXT("E_Weapon case"));
			if (SearchSound->IsValidLowLevelFast())
				UGameplayStatics::PlaySoundAtLocation(this, SearchSound, GetActorLocation());
			SelectWeapon();
			Destroy();
			break;
		case EItemCategoryInfo::E_Bullet:
			UE_LOG(LogTemp, Log, TEXT("E_BulletCase"));
			if (SearchSound->IsValidLowLevelFast())
				UGameplayStatics::PlaySoundAtLocation(this, SearchSound, GetActorLocation());
			SelectProjectile();
			Destroy();
			break;
		case EItemCategoryInfo::E_Buff:
			Stat = DA->BuffStat;
			MyCharacter->SetBuffTimer(Stat.Type, this, Stat.Time, Stat.Time);
			Destroy();
			break;
		case EItemCategoryInfo::E_DeBuff:
			break;
		case EItemCategoryInfo::E_StatUp:
			break;
		case EItemCategoryInfo::E_Mission:
			InGameScriptRef->ChapterManager->RemoveMissionItem(this);
			Destroy();
			break;
		default:
			break;
		}
	}
}

void AItemBase::InitItem(EItemCategoryInfo setType)
{
	Type = setType;

}

void AItemBase::SelectWeapon()
{
	int8 weaponType = FMath::RandRange(0, MaxWeaponType - 1);
	int32 weaponID = 0;
	switch (weaponType)
	{
	case 0:
		weaponID = 100;
		break;
	case 1:
		weaponID = 101;
		break;
	case 2:
		weaponID = 102;
		break;
	case 3:
		weaponID = 103;
		break;
	case 4:
		weaponID = 151;
		break;
	case 5:
		weaponID = 199;
		break;
	default:
		break;
	}
	MyCharacter->PickWeapon(weaponID);
}

void AItemBase::SelectProjectile()
{
	int8 ProjectileType = FMath::RandRange(0, MaxProjectileType - 1);

	int32 ProjectileID = 0;
	switch (ProjectileType)
	{
	case 0:
		ProjectileID = 200;
		break;
	case 1:
		ProjectileID = 204;
		break;
	default:
		break;
	}
}