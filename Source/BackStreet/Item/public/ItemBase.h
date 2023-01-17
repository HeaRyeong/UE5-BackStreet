// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ItemBase.generated.h"

UCLASS()
class BACKSTREET_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();
	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OverlapVolume;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* BuffItemTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* WeaponItemTable;

	// ÂüÁ¶
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* TileRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABackStreetGameModeBase* GameModeRef;
};
