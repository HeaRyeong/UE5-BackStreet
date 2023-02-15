// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ItemBoxBase.h"
#include "../public/ItemBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
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

	//�̼� ������ ����� �߰�
	SpawnItemTypeList.Add(EItemCategoryInfo::E_Mission);
	SpawnItemIDList.Add(0);
	ItemSpawnProbabilityList.Add(100.0f);
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
	if (MinSpawnItemCount > MaxSpawnItemCount) //���� ������ ����� �̷����� ����
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemBoxBase::OnItemBoxOpened) SpawnCount Info�� �ùٸ��� �ʽ��ϴ�."));
		return;
	}

	int32 spawnItemCount = UKismetMathLibrary::RandomIntegerInRange(MinSpawnItemCount, MaxSpawnItemCount);
	TArray<AItemBase*> spawnedItemList = SpawnItems(spawnItemCount);

	for (auto& itemRef : spawnedItemList)
	{
		if (!IsValid(itemRef)) continue;
		LaunchItem(itemRef);
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OpenEffectParticle, GetActorLocation(), FRotator(), FVector(1.5f));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenEffectSound, GetActorLocation());
	Destroy();
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

TArray<AItemBase*> AItemBoxBase::SpawnItems(int32 TargetSpawnCount)
{
	//������Ƽ �� �� �迭 ������ �ùٸ��� ���� ���
	if (SpawnItemTypeList.Num() != SpawnItemIDList.Num() || SpawnItemIDList.Num() != ItemSpawnProbabilityList.Num()) return TArray<AItemBase*>();

	TArray<AItemBase*> itemList; //��ȯ�� ������ ����Ʈ 
	TSet<int32> duplicateCheckSet; //�ߺ� üũ Set

	bool bMissionItemFlag = !bIncludeMissionItem; //�̼� ������ ���� üũ, ������ �ڽ� �� �ִ� 1��
	int32 currentSpawnCount = 0;
	int32 currentTryCount = 0;

	while(currentSpawnCount < TargetSpawnCount)
	{
		if (++currentTryCount >= MAX_TRY_SPAWN_COUNT) break;

		int32 targetItemInfoIdx = !bMissionItemFlag ? 0 : UKismetMathLibrary::RandomIntegerInRange(0, SpawnItemTypeList.Num()-1);
		const int32 targetItemType = (int32)SpawnItemTypeList[targetItemInfoIdx];
		const int32 targetItemID = SpawnItemIDList[targetItemInfoIdx];
		const float targetItemProbability = ItemSpawnProbabilityList[targetItemInfoIdx];
		const int32 targetItemKey = targetItemType * 1000 + targetItemID; //Set�� �־� �ߺ� üũ�� �Ҷ� ����� ���� Ű ��.

		//�̹� �ش� �������� �����ߴٸ�? �ٽ� ����
		if (duplicateCheckSet.Contains(targetItemKey)) continue;

		if (UKismetMathLibrary::RandomIntegerInRange(0, 100) <= targetItemProbability)
		{
			const float randomValue = UKismetMathLibrary::RandomFloatInRange(-10.0f, 10.0f);
			const FVector spawnLocation = GetActorLocation() + FVector(randomValue, randomValue, currentSpawnCount * SpawnLocationInterval + 125.0f);
			AItemBase* newItem = GamemodeRef->SpawnItemToWorld(targetItemType, targetItemID, spawnLocation);

			if (IsValid(newItem))
			{
				itemList.Add(newItem);
				newItem->SetActorScale3D(FVector(0.1));
				newItem->InitItem((EItemCategoryInfo)targetItemType, targetItemID);
				bMissionItemFlag = (bMissionItemFlag || (EItemCategoryInfo)targetItemType == EItemCategoryInfo::E_Mission);

				currentSpawnCount++;
				duplicateCheckSet.Add(targetItemKey); //���������� ������ �������� �¿� �־� ����
			}
		}
	}
	return itemList;
}

void AItemBoxBase::LaunchItem(AItemBase* TargetItem)
{
	if (!IsValid(TargetItem)) return;

	FVector itemLocation = TargetItem->GetActorLocation(); 
	FVector launchDirection = FMath::VRandCone(GetActorUpVector(), CornHalfRadius);
	//DrawDebugLine(GetWorld(), itemLocation, itemLocation + launchDirection * 100.0f, FColor::MakeRandomColor(), false, 20.0f, 0, 5.0f);

	TargetItem->SetLaunchDirection(launchDirection * LaunchSpeed);
	TargetItem->ActivateProjectileMovement();
	TargetItem->ActivateItem();
}
