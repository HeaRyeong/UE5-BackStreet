// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ItemBoxBase.generated.h"
#define MAX_TRY_SPAWN_COUNT 25

DECLARE_DELEGATE_OneParam(FDeleAddMissionItem, class AItemBase*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeleOpenItemBox, AActor*, Owner);

UCLASS()
class BACKSTREET_API AItemBoxBase : public AActor
{
	GENERATED_BODY()

// ----- Delegate ---------------------------
public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FDeleOpenItemBox OnPlayerOpenBegin;

	FDeleAddMissionItem OnMissionItemSpawned;

// ----- Global, Component ------------------
public:	
	AItemBoxBase();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* OverlapVolume;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
		class UNiagaraComponent* ParticleComponent;

// ------ �⺻ ���� -----------------------------
public:
	// �ܺο��� Init�ϱ����� Call
	UFUNCTION()
		void InitItemBox(bool _bIncludeMissionItem);

	UFUNCTION()
		void OnItemBoxOpened(AActor* Causer);

	UFUNCTION()
		void OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	//�ִ� MaxSpawnCount ��ŭ�� �������� Ȯ���� ����Ͽ� �����Ͽ� List ���·� ��ȯ
	UFUNCTION()
		TArray<class AItemBase*> SpawnItems(int32 TargetSpawnCount);

	UFUNCTION()
		void LaunchItem(class AItemBase* TargetItem);

// ------ �⺻ Property ---------------------------
private:
	UPROPERTY()
		bool bIncludeMissionItem = false;

protected:
	//�ִ�� ������ ������ ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings", meta = (UIMin = 1, UIMax = 10))
		int32 MaxSpawnItemCount;

	//�ּҷ� ������ ������ ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings", meta = (UIMin = 1, UIMax = 10))
		int32 MinSpawnItemCount;

	//���� ������ �ڽ��� ���� ������ �������� Category Type ����Ʈ : idx���� �������� ����  / �̼� �������� 0���� �ʼ��� ����!!
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<EItemCategoryInfo> SpawnItemTypeList;

	//���� ������ �ڽ��� ���� ������ �������� ID ����Ʈ : idx���� �������� ���� / �̼� �������� 0���� �ʼ��� ����!!
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<int32> SpawnItemIDList;

	//�� �������� ���� Ȯ���� ���� ����Ʈ (0 ~ 100)  / �̼� �������� �ʼ��� ����!!
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<float> ItemSpawnProbabilityList;

	UFUNCTION()
		bool GetMissionItemIsIncluded() { return bIncludeMissionItem; }

// ----- Item Launch Option --------------------------
protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay|Settings|Launch")
		float CornHalfRadius = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Gameplay|Settings|Launch")
		float LaunchSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Gameplay|Settings|Launch")
		float SpawnLocationInterval = 15.0f;

// ----- Asset -----------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		class UParticleSystem* OpenEffectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* OpenEffectSound;

// ----- Ref -----------------------------------
private:
	TWeakObjectPtr<class ABackStreetGameModeBase> GamemodeRef;
};
