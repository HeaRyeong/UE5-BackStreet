// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/RewardBox.h"
#include "Components/SphereComponent.h"
// Sets default values
ARewardBox::ARewardBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = OverlapVolume = CreateDefaultSubobject<USphereComponent>("SPHERE_COLLISION");
	OverlapVolume->SetRelativeScale3D(FVector(5.0f));
	OverlapVolume->SetCollisionProfileName("ItemTrigger", true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM_MESH"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("Item", false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

// Called when the game starts or when spawned
void ARewardBox::BeginPlay()
{
	Super::BeginPlay();

	OpenUIDelegate.AddDynamic(this, &ARewardBox::EnterUI);
	AbilitySelect();
	
}

// Called every frame
void ARewardBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ARewardBox::OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;
//
//	//UI Activate
//}
//
//void ARewardBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;
//
//	//UI Deactivate
//}

void ARewardBox::AbilitySelect()
{
	const TArray<int32> AbilityTypeID = {0,1,2,3,4,5,6,7,8};
	int32 AbilityIdx = FMath::RandRange(0, AbilityTypeID.Num() - 1);

	PossessAbilityID = AbilityTypeID[AbilityIdx];
}

void ARewardBox::TrySwapAbility(int32 GetAbility, int32 StoreAbility)
{
	// Check Right Swapping
	// Call SwapAbility
	SwapAbility(GetAbility, StoreAbility);
}

void ARewardBox::SwapAbility(int32 GetAbility, int32 StoreAbility)
{
	// Call MainCharacter Ability Swap Request
}