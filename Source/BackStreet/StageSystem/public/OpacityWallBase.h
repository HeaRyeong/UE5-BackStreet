// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "OpacityWallBase.generated.h"

UCLASS()
class BACKSTREET_API AOpacityWallBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOpacityWallBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	//UFUNCTION(BlueprintCallable)
	//	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	//UFUNCTION(BlueprintCallable)
	//	void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetShowSeeingThrough(bool bThroughShow);

public:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* SeeingTroughMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* OverlapVolume;
	UPROPERTY(EditAnywhere)
		float SeeingThroughTime;
	UPROPERTY(EditAnywhere)
		bool bSeeingThrough;
	UPROPERTY(EditAnywhere)
		float RunningTimer;

};
