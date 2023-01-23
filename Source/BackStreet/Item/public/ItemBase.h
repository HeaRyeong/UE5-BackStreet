// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ItemInfoStruct.h"
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
	// 외부에서 Init하기위해 Call
	UFUNCTION()
		void InitItem(EItemCategoryInfo SetType);

public:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		class UBoxComponent* OverlapVolume;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		class UNiagaraComponent* NiagaraCompo;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		class UItemDataAssetInfo* DA;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemCategoryInfo Type = EItemCategoryInfo::E_None;

	// 참조
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* TileRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABackStreetGameModeBase* GameModeRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAssetManagerBase* AssetManagerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACharacterBase* MyCharacter;
};
