// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Item.generated.h"

UCLASS()
class BACKSTREET_API AItem : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OverlapVolume;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* BuffItemTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* WeaponItemTable;

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// 소속 타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATile* TileRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABackStreetGameModeBase* GameModeRef;
};
