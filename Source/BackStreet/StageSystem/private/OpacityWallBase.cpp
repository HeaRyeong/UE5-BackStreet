// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/OpacityWallBase.h"
#include "Components/BoxComponent.h"



// Sets default values
AOpacityWallBase::AOpacityWallBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    DynamicMaterialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DynamicMaterialMesh"));
    OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
    OverlapVolume->SetupAttachment(RootComponent);
    DynamicMaterialMesh->SetupAttachment(RootComponent);
    //RootComponent = OverlapVolume;
    /*OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &AOpacityWallBase::OverlapBegins);
    
    OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &AOpacityWallBase::OverlapEnds);*/

    ChaningTime = 0.3f;
    TimeTotal = 0.0f;
    bIsVisible = false;
    bSetVisible = true;
}

// Called when the game starts or when spawned
void AOpacityWallBase::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called every frame
void AOpacityWallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TimeTotal += DeltaTime;

    float Opacity = 0.0f;
    if (bIsVisible)
    {
        Opacity = FMath::Lerp(1.0f, 0.0f, TimeTotal * (1.0f / ChaningTime));
    }
    else
    {
        Opacity = FMath::Lerp(0.0f, 1.0f, TimeTotal * (1.0f / ChaningTime));
    }

    DynamicMaterialMesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), Opacity);

    if (TimeTotal > ChaningTime)
    {
        TimeTotal = 0.0f;
        PrimaryActorTick.SetTickFunctionEnable(false);
    }
}

void AOpacityWallBase::SetShowSeeingThrough(bool bThroughShow)
{
    bIsVisible = bThroughShow;
  
    if (TimeTotal != 0.0f)
    {
        TimeTotal = ChaningTime - TimeTotal;
    }
    PrimaryActorTick.SetTickFunctionEnable(true);
}

//void AOpacityWallBase::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    if (OtherActor->ActorHasTag("Player"))
//    {
//        SetShowSeeingThrough(true);
//    }
//}
//
//void AOpacityWallBase::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    if (OtherActor->ActorHasTag("Player"))
//    {
//        SetShowSeeingThrough(false);
//     
//    }
//}