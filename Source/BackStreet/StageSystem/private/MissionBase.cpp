// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MissionBase.h"

// Sets default values
AMissionBase::AMissionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void AMissionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called when the game starts or when spawned
void AMissionBase::BeginPlay()
{
	Super::BeginPlay();
	Type = FMath::RandRange(0, 1);
	UE_LOG(LogTemp, Log, TEXT("Spawn Mission %d"),Type);
}

