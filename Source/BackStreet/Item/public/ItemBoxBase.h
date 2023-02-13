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
		void InitItemBox();

	UFUNCTION()
		void OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// ------ �⺻ Property ---------------------------
protected:
	//�ִ�� ������ ������ ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		int32 MaxSpawnItemCount;

	//�ּҷ� ������ ������ ����
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		int32 MinSpawnItemCount;

	//������ ������ Ŭ������ ����Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Settings")
		TArray<TSubclassOf<class AItemBase>> SpawnItemClassList;

	//�� �������� ���� Ȯ���� ���� ����Ʈ
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
