// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/RewardBox.h"
#include "Components/SphereComponent.h"
#include "../public/TileBase.h"
#include "../../Character/public/MainCharacterBase.h"
#include "../../Character/public/AbilityManagerBase.h"
#include "../../Character/public/CharacterInfoEnum.h"


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

void ARewardBox::AbilitySelect()
{
	const TArray<int32> AbilityTypeID = {0,1,2,3,4,5,6,7,8};
	int32 AbilityIdx = FMath::RandRange(1, AbilityTypeID.Num()-1);

	PossessAbilityID = AbilityTypeID[AbilityIdx];
	CharacterAbilityIDA = CharacterAbilityIDB = 0;
	SetCharacterAbilityList();
}

bool ARewardBox::TrySwapAbility(int32 GetAbility, int32 StoreAbility)
{
	// Check Right Swapping
	// Call SwapAbility

	AMainCharacterBase* characterRef = Cast<AMainCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	UAbilityManagerBase* abilityManagerRef = characterRef->GetAbilityManager();
	TArray<ECharacterAbilityType> abilityList = abilityManagerRef->GetActiveAbilityList();

	if (GetAbility == StoreAbility)
		return false;

	for (ECharacterAbilityType type : abilityList)
	{
		if (GetAbility == (int32)type)
			return false;
	}

	if (StoreAbility != 0 && GetAbility == 0)
	{
		PossessAbilityID = StoreAbility;
		abilityManagerRef->TryRemoveAbility(((ECharacterAbilityType)StoreAbility));
		SetCharacterAbilityList();
		UpdateUI();
		return true;
	}

	if (abilityList.Num() == 0 || abilityList.Num()==1)
	{
		PossessAbilityID = 0;
		abilityManagerRef->TryAddNewAbility(((ECharacterAbilityType)GetAbility));
		SetCharacterAbilityList();
		UpdateUI();
		return true;
	}
	else if (abilityList.Num() == 2)
	{
		abilityManagerRef->TryRemoveAbility(((ECharacterAbilityType)StoreAbility));
		abilityManagerRef->TryAddNewAbility(((ECharacterAbilityType)GetAbility));
		PossessAbilityID = StoreAbility;
		SetCharacterAbilityList();
		UpdateUI();
		return true;
	}
	else
		return false;

}


void ARewardBox::SetBelongTile(ATileBase* Target)
{
	BelongTile = Target;
}

void ARewardBox::SetCharacterAbilityList()
{
	AMainCharacterBase* characterRef = Cast<AMainCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	UAbilityManagerBase* abilityManagerRef = characterRef->GetAbilityManager();
	TArray<ECharacterAbilityType> abilityList = abilityManagerRef->GetActiveAbilityList();

	if (abilityList.Num() == 2)
	{
		CharacterAbilityIDA = (int32)abilityList[0];
		CharacterAbilityIDB = (int32)abilityList[1];

	}
	else if (abilityList.Num() == 1)
	{
		CharacterAbilityIDA = (int32)abilityList[0];
		CharacterAbilityIDB = 0;
	}
	else
	{
		CharacterAbilityIDA = CharacterAbilityIDB = 0;
	}
	
}