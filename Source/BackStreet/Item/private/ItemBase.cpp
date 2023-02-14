// Fill out your copyright notice in the Description page of Project Settings.
#include "../public/ItemBase.h"
#include "../public/WeaponBase.h"
#include "Components/WidgetComponent.h"
#include "../../StageSystem/public/MissionBase.h"
#include "../../StageSystem/public/TileBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/ChapterManagerBase.h"
#include "../../StageSystem/public/ALevelScriptInGame.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "../../Character/public/CharacterBuffManager.h"

// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->Tags.Add(FName("Item"));

	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENEROOT"));
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM_MESH"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ITEM_INFO_WIDGET"));
	InfoWidgetComponent->SetupAttachment(MeshComponent);
	InfoWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	InfoWidgetComponent->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));

	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ITEM_NIAGARA_COMPONENT"));
	ParticleComponent->SetupAttachment(RootComponent);

	OverlapVolume = CreateDefaultSubobject<USphereComponent>("SPHERE_COLLISION");
	OverlapVolume->SetupAttachment(RootComponent);
	OverlapVolume->SetRelativeScale3D(FVector(5.0f));
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AItemBase::OnOverlapBegins);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());
	OnPlayerBeginPickUp.BindUFunction(this, FName("OnItemPicked"));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
}

void AItemBase::InitItem(EItemCategoryInfo SetType)
{
	ItemType = SetType;

	if (SetType == EItemCategoryInfo::E_Mission)
	{
		//GamemodeRef->GetCurrentTileRef()->MissionInfo->ItemList.AddUnique(this);
	}
}

void AItemBase::OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;

	ParticleComponent->Activate();
}

void AItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;

	ParticleComponent->Deactivate();
}
/*
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
}*/