#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RewardBoxBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateOpenUI);


UCLASS()
class BACKSTREET_API ARewardBoxBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDelegateOpenUI OpenUIDelegate;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* OverlapVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Mesh;

public:
	// Sets default values for this actor's properties
	ARewardBoxBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void SelectRandomAbilityIdx();

	UFUNCTION(BlueprintCallable)
		bool TrySwapAbility(int32 GetAbility, int32 StoreAbility);

	UFUNCTION()
		void SetBelongTile(ATileBase* Target);

	UFUNCTION(BlueprintCallable)
		void SetCharacterAbilityList();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 PossessAbilityID;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ATileBase* BelongTile;
};
