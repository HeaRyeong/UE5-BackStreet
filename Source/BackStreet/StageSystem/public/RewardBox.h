// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RewardBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateOpenUI);


UCLASS()
class BACKSTREET_API ARewardBox : public AActor
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PossessAbilityID;
	
public:	
	// Sets default values for this actor's properties
	ARewardBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	/*UFUNCTION()
		void OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

	UFUNCTION(BlueprintImplementableEvent)
		void EnterUI();

	UFUNCTION(BlueprintCallable)
		void AbilitySelect();

	UFUNCTION(BlueprintCallable)
		void TrySwapAbility(int32 GetAbility, int32 StoreAbility);

	UFUNCTION(BlueprintCallable)
		void SwapAbility(int32 GetAbility, int32 StoreAbility);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateUI();
};