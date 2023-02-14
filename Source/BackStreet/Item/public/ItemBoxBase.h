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

private:
	//�ִ� MaxSpawnCount ��ŭ�� �������� Ȯ���� ����Ͽ� �����Ͽ� List ���·� ��ȯ
	UFUNCTION()
		TArray<class AItemBase*> SpawnItems(int32 MaxSpawnCount);

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

	//���� ������ �ڽ��� ���� ������ �������� Category Type ����Ʈ : idx���� �������� ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<EItemCategoryInfo> SpawnItemTypeList;

	//���� ������ �ڽ��� ���� ������ �������� ID ����Ʈ : idx���� �������� ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<int32> SpawnItemIDList;

	//�� �������� ���� Ȯ���� ���� ����Ʈ (0 ~ 100)
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
