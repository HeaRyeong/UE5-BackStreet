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
#include "GameFramework/ProjectileMovementComponent.h"

#define DEFAULT_ITEM_LAUNCH_SPEED 500.0f

// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->Tags.Add(FName("Item"));

	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENEROOT"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM_MESH"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName("Item", false);
	//MeshComponent->SetSimulatePhysics(true);

	InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ITEM_INFO_WIDGET"));
	InfoWidgetComponent->SetupAttachment(MeshComponent);
	InfoWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	InfoWidgetComponent->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));

	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ITEM_NIAGARA_COMPONENT"));
	ParticleComponent->SetupAttachment(MeshComponent);

	OverlapVolume = CreateDefaultSubobject<USphereComponent>("SPHERE_COLLISION");
	OverlapVolume->SetupAttachment(MeshComponent);
	OverlapVolume->SetRelativeScale3D(FVector(5.0f));
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AItemBase::OnOverlapBegins);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE_MOVEMENT"));
	ProjectileMovement->InitialSpeed = DEFAULT_ITEM_LAUNCH_SPEED;
	ProjectileMovement->MaxSpeed = DEFAULT_ITEM_LAUNCH_SPEED;
	ProjectileMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());
	OnPlayerBeginPickUp.BindUFunction(this, FName("OnItemPicked"));
	InGameScriptRef = Cast<ALevelScriptInGame>(GetWorld()->GetLevelScriptActor(GetWorld()->GetCurrentLevel()));
}

void AItemBase::InitItem(EItemCategoryInfo SetType, uint8 NewItemID)
{
	ItemType = SetType;
	ItemID = NewItemID;

	if (ItemType == EItemCategoryInfo::E_Mission)
	{
		bool result = InGameScriptRef->ChapterManager->TryAddMissionItem(this);

		if (!result)
		{
			UE_LOG(LogTemp, Warning, TEXT("최대 미션 아이템 개수를 초과했습니다."));
		}
	}
}

void AItemBase::OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;

	ParticleComponent->Activate();
	//UI Activate
}

void AItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;

	ParticleComponent->Deactivate();
	//UI Deactivate
}

void AItemBase::SetLaunchDirection(FVector NewDirection)
{
	ProjectileMovement->Velocity = NewDirection;
}

void AItemBase::ActivateProjectileMovement()
{
	ProjectileMovement->Activate();
}