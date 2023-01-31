// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

#define MaxWeaponType 6
#define MaxProjectileType 2

UCLASS()
class BACKSTREET_API AItemBase : public AActor
{
	GENERATED_BODY()

// ------ Global ---------------------------------------------
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

// ------ 아이템 기본 로직-------------------------------------
public:
	// 외부에서 Init하기위해 Call
	UFUNCTION()
		void InitItem(EItemCategoryInfo SetType);

	UFUNCTION()
		void SelectWeapon();

	UFUNCTION()
		void SelectProjectile();

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

// ------ 참조 프로퍼티 ---------------------------------------------
private:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		TArray<TSubclassOf<class AWeaponBase> > WeaponClassList;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		TArray<int32> WeaponIDList;

	UPROPERTY(VisibleDefaultsOnly)
		class ATileBase* TileRef;

	UPROPERTY(VisibleDefaultsOnly)
		class ABackStreetGameModeBase* GameModeRef;

	UPROPERTY(VisibleDefaultsOnly)
		class AAssetManagerBase* AssetManagerRef;

	UPROPERTY(VisibleDefaultsOnly)
		class ACharacterBase* MyCharacter;
};
