// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ItemBoxBase.h"
#include "../public/ItemBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RandomStream.h"

// Sets default values
AItemBoxBase::AItemBoxBase()
{
	this->Tags.Add(FName("ItemBox"));
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM_MESH"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName("Item", false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	ParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ITEM_NIAGARA_COMPONENT"));
	ParticleComponent->SetupAttachment(RootComponent);

	OverlapVolume = CreateDefaultSubobject<USphereComponent>("SPHERE_COLLISION");
	OverlapVolume->SetupAttachment(RootComponent);
	OverlapVolume->SetRelativeScale3D(FVector(5.0f));
	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &AItemBoxBase::OnOverlapBegins);
}

// Called when the game starts or when spawned
void AItemBoxBase::BeginPlay()
{
	Super::BeginPlay();
	OnPlayerOpenBegin.BindUFunction(this, FName("OnItemBoxOpened"));

	GamemodeRef = Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode());
}

void AItemBoxBase::InitItemBox(bool _bIncludeMissionItem)
{	
	bIncludeMissionItem = _bIncludeMissionItem;
}

void AItemBoxBase::OnItemBoxOpened(AActor* Causer)
{
	if (!IsValid(Causer) || !Causer->ActorHasTag("Player")) return;
	if (MinSpawnItemCount > MaxSpawnItemCount) //정보 기입이 제대로 이뤄지지 않음
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemBoxBase::OnItemBoxOpened) SpawnCount Info가 올바르지 않습니다."));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Open #3"));

	int32 spawnItemCount = UKismetMathLibrary::RandomIntegerInRange(MinSpawnItemCount, MaxSpawnItemCount);
	TArray<AItemBase*> spawnedItemList = SpawnItems(spawnItemCount);

	UE_LOG(LogTemp, Warning, TEXT("Open #4"));

	for (auto& itemRef : spawnedItemList)
	{
		if (!IsValid(itemRef)) continue;

		UE_LOG(LogTemp, Warning, TEXT("Open #5"));

		LaunchItem(itemRef);
	}
}

void AItemBoxBase::OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;

	//UI Activate
}

void AItemBoxBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || OtherActor->ActorHasTag("Player")) return;

	//UI Deactivate
}

TArray<AItemBase*> AItemBoxBase::SpawnItems(int32 MaxSpawnCount)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn #0"));
	if (SpawnItemTypeList.Num() != SpawnItemIDList.Num() || SpawnItemIDList.Num() != ItemSpawnProbabilityList.Num()) return TArray<AItemBase*>();

	UE_LOG(LogTemp, Warning, TEXT("Spawn #1"));

	TArray<AItemBase*> itemList;
	bool bMissionItemFlag = !bIncludeMissionItem; //미션 아이템 스폰 체크, 아이템 박스 당 최대 1개
	
	for (int itemIdx = 0; itemIdx < MaxSpawnCount; itemIdx += 1)
	{
		const int32 targetItemInfoIdx = UKismetMathLibrary::RandomIntegerInRange(0, SpawnItemTypeList.Num()-1);
		const int32 targetItemType = (int32)SpawnItemTypeList[targetItemInfoIdx];
		const int32 targetItemID = SpawnItemIDList[targetItemInfoIdx];
		const float targetItemProbability = ItemSpawnProbabilityList[targetItemInfoIdx];

		//이미 미션 아이템을 스폰했다면? 다시 선택
		if ((EItemCategoryInfo)targetItemType == EItemCategoryInfo::E_Mission && bMissionItemFlag)
		{
			itemIdx -= 1; 
			continue;
		}

		if (UKismetMathLibrary::RandomIntegerInRange(0, 100) <= targetItemProbability)
		{
			AItemBase* newItem = GamemodeRef->SpawnItemToWorld(targetItemType, targetItemID, GetActorLocation());

			if (IsValid(newItem))
			{
				itemList.Add(newItem);
				newItem->InitItem((EItemCategoryInfo)targetItemType, targetItemID);
				bMissionItemFlag = (bMissionItemFlag || (EItemCategoryInfo)targetItemType == EItemCategoryInfo::E_Mission);
			}
		}
	}
	return itemList;
}

void AItemBoxBase::LaunchItem(AItemBase* TargetItem)
{
	if (!IsValid(TargetItem)) return;

	FVector itemLocation = TargetItem->GetActorLocation(); 
	FVector launchDirection = FMath::VRandCone(GetActorUpVector(), 2.5f);
	FRotator launchRotation = UKismetMathLibrary::FindLookAtRotation(itemLocation, itemLocation + launchDirection * 20.0f);

	DrawDebugLine(GetWorld(), itemLocation, itemLocation + launchDirection * 100.0f, FColor::MakeRandomColor(), false, 20.0f, 0, 5.0f);

	TargetItem->SetLaunchDirection(launchDirection * 500.0f);
	TargetItem->SetActorRotation(launchRotation);
	TargetItem->ActivateProjectileMovement();
}
