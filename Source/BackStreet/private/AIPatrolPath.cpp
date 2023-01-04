// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
AAIPatrolPath::AAIPatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PATH_SPLINE"));
	PathSpline->SetupAttachment(RootComponent);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("BILLBOARD"));
	Billboard->SetupAttachment(RootComponent);
	Billboard->SetRelativeScale3D(FVector(0.5f));
}

// Called when the game starts or when spawned
void AAIPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIPatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

