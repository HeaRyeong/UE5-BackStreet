// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "LevelStreamerActor.generated.h"

UCLASS()
class BACKSTREET_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OverlapVolume;

	UPROPERTY(EditAnywhere)
		FName LevelToLoad;

	UPROPERTY(EditAnywhere)
		bool IsInside = false;

public:
	// Sets default values for this actor's properties
	ALevelStreamerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetLevelToLoad(FName Level, FVector WorldOffset, bool bActorsOnly);
};
