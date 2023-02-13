// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/ItemBoxBase.h"

// Sets default values
AItemBoxBase::AItemBoxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM_MESH"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	OnPlayerOpenBegin.BindUFunction(this, FName(""));
}

void AItemBoxBase::InitItemBox()
{
}

void AItemBoxBase::OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AItemBoxBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
