// Fill out your copyright notice in the Description page of Project Settings.

#include "../public/OpacityWallBase.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Components/BoxComponent.h"



// Sets default values
AOpacityWallBase::AOpacityWallBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
    SeeingTroughMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SeeingThroughMesh"));

    SeeingTroughMesh->CreateDynamicMaterialInstance(0);
    SeeingTroughMesh->SetCollisionProfileName(TEXT("InvisibleWall"));
    OverlapVolume->SetupAttachment(RootComponent);
    SeeingTroughMesh->SetupAttachment(RootComponent);
    SeeingThroughTime = 0.3f;
    RunningTimer = 0.0f;
    bSeeingThrough = true;
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

    RunningTimer += DeltaTime;

    float Opacity = 0.0f;
    if (bSeeingThrough)
    {
        Opacity = FMath::Lerp(1.0f, 0.0f, RunningTimer * (1.0f / SeeingThroughTime)); // 사라짐
        UE_LOG(LogTemp, Log, TEXT("True : %f"), Opacity);
    }
    else
    {
        Opacity = FMath::Lerp(0.0f, 1.0f, RunningTimer * (1.0f / SeeingThroughTime)); // 나타남
        UE_LOG(LogTemp, Log, TEXT("false : %f"), Opacity);
    }

    SeeingTroughMesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), Opacity); 

    if (RunningTimer > SeeingThroughTime)
    {
        RunningTimer = 0.0f;
        PrimaryActorTick.SetTickFunctionEnable(false);
    }
}
//         RunningTimer == 0 사라지고 나타나는게 끝났다 -> opacity 작업이 멈춰야한다.

void AOpacityWallBase::SetShowSeeingThrough(bool bThroughShow)
{
    bSeeingThrough = bThroughShow;
   /* if (RunningTimer != 0.0f)
    {
        RunningTimer = SeeingThroughTime - RunningTimer;
    }*/
        PrimaryActorTick.SetTickFunctionEnable(true);
    
}

void AOpacityWallBase::GetMaterial()
{
    TArray<UMaterialInterface*> Materials = SeeingTroughMesh->GetMaterials();
  
}