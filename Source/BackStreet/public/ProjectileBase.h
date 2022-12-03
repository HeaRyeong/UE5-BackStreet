// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "GameFramework/Actor.h"
#include "WeaponStatStructBase.h"
#include "ProjectileBase.generated.h"


UCLASS()
class BACKSTREET_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void OnProjectileBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
								, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void ActivateProjectileMovement();

	UFUNCTION()
		void SetSpawnInstigator(AController* NewInstigator);

public:	
	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* Mesh; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UProjectileMovementComponent* ProjectileMovement;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
		UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		struct FProjectileStatStruct ProjectileStat;

private: 
	UPROPERTY()
		AController* SpawnInstigator;
};
