// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

#define MAX_WEAPON_TYPE 6
#define MAX_PROJECTILE_TYPE 2

DECLARE_DELEGATE_OneParam(FDelePickItem, AActor*);

UCLASS()
class BACKSTREET_API AItemBase : public AActor
{
	GENERATED_BODY()
public:
	FDelePickItem OnPlayerBeginPickUp;

// ------ Global, Component ---------------------------------------------
public:
	AItemBase();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		class USphereComponent* OverlapVolume;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
		class UWidgetComponent* InfoWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
		class UNiagaraComponent* ParticleComponent;

// ------ �⺻ Info ---------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemCategoryInfo ItemType = EItemCategoryInfo::E_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		uint8 ItemID;
// ------ ������ �⺻ ����-------------------------------------
public:
	// �ܺο��� Init�ϱ����� Call
	UFUNCTION()
		void InitItem(EItemCategoryInfo SetType);

	UFUNCTION()
		void OnOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//ĳ���Ͱ� Pick�̺�Ʈ�� ȣ���ߴٸ�
	UFUNCTION(BlueprintImplementableEvent)
		void OnItemPicked(AActor* Causer);

// ------ Asset ----------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* SearchSound;

// ------ ���� ������Ƽ ---------------------------------------------
private:
	UPROPERTY(VisibleDefaultsOnly)
		class ABackStreetGameModeBase* GameModeRef;
};
