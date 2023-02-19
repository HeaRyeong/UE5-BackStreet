// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "Components/BoxComponent.h"
#include "GateBase.generated.h"

UCLASS()
class BACKSTREET_API AGateBase : public AActor
{
	GENERATED_BODY()


//------------ Global / Component -------------------
public:
	// Sets default values for this actor's properties
	AGateBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OverlapVolume;

//----------- 핵심 로직 ---------------
public:
	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void InitGate();

	UFUNCTION(BlueprintCallable)
		void EnterGate();

public:
	//새 타일을 로드 / 기존 타일을 언로드 한다.
	UFUNCTION()
		void UpdateNewTile();

	UFUNCTION(BlueprintCallable)
		void CheckHaveToActive();



//-------- 그 외-----------------------
private:
	UPROPERTY()
		class ALevelScriptInGame* InGameScriptRef;
};
