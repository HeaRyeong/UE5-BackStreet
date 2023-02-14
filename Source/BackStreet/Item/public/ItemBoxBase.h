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
		void InitItemBox(bool _bIncludeMissionItem);

	UFUNCTION()
		void OnItemBoxOpened(AActor* Causer);

	UFUNCTION()
		void OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	//최대 MaxSpawnCount 만큼의 아이템을 확률에 기반하여 스폰하여 List 형태로 반환
	UFUNCTION()
		TArray<class AItemBase*> SpawnItems(int32 MaxSpawnCount);

	UFUNCTION()
		void LaunchItem(class AItemBase* TargetItem);

// ------ 기본 Property ---------------------------
private:
	UPROPERTY()
		bool bIncludeMissionItem = false;

protected:
	//최대로 스폰할 아이템 개수
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings", meta = (UIMin = 1, UIMax = 10))
		int32 MaxSpawnItemCount;

	//최소로 스폰할 아이템 개수
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings", meta = (UIMin = 1, UIMax = 10))
		int32 MinSpawnItemCount;

	//현재 아이템 박스가 스폰 가능한 아이템의 Category Type 리스트 : idx별로 아이템을 구분
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<EItemCategoryInfo> SpawnItemTypeList;

	//현재 아이템 박스가 스폰 가능한 아이템의 ID 리스트 : idx별로 아이템을 구분
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<int32> SpawnItemIDList;

	//각 아이템의 등장 확률를 담은 리스트 (0 ~ 100)
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<float> ItemSpawnProbabilityList;

	UFUNCTION()
		bool GetMissionItemIsIncluded() { return bIncludeMissionItem; }

// ----- Asset -----------------------------------
protected:
	//UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		//UNiagaraComponent* OpenEffectParticle


// ----- Ref -----------------------------------
private:
	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

	UPROPERTY()
		class ALevelScriptInGame* InGameScriptRef;
};
