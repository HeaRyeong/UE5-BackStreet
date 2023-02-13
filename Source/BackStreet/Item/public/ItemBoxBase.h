// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ItemBoxBase.generated.h"

DECLARE_DELEGATE_OneParam(FDeleOpenItemBox, AActor*);

UCLASS()
class BACKSTREET_API AItemBoxBase : public AActor
{
	GENERATED_BODY()

// ----- Delegate ---------------------------
public:
	FDeleOpenItemBox OnPlayerOpenBegin;

// ----- Global, Component ------------------
public:	
	AItemBoxBase();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		class USphereComponent* OverlapVolume;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
		class UNiagaraComponent* ParticleComponent;

// ------ 기본 로직 -----------------------------
public:
	// 외부에서 Init하기위해 Call
	UFUNCTION()
		void InitItemBox();

	UFUNCTION()
		void OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// ------ 기본 Property ---------------------------
protected:
	//최대로 스폰할 아이템 개수
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		int32 MaxSpawnItemCount;

	//최소로 스폰할 아이템 개수
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		int32 MinSpawnItemCount;

	//스폰할 아이템 클래스의 리스트
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<TSubclassOf<class AItemBase>> SpawnItemClassList;

	//각 아이템의 등장 확률를 담은 리스트
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<float> ItemSpawnProbabilityList;


// ----- Asset -----------------------------------
protected:
	//UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		//UNiagaraComponent* OpenEffectParticle


// ----- Ref -----------------------------------
private:
	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;
};
