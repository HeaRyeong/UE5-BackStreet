// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"


UCLASS()
class BACKSTREET_API AProjectileBase : public AActor
{
	GENERATED_BODY()

	friend class AWeaponBase;

//------ Global, Component -------------------
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent* TargetingCollision;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UProjectileMovementComponent* ProjectileMovement;

//------- �⺻ ������Ƽ -------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		uint8 ProjectileID;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		USoundCue* HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		USoundCue* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|VFX")
		UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		struct FProjectileStatStruct ProjectileStat;

//------ �ٽ� �Լ�  ------------------
public:
	UFUNCTION()
		void InitProjectile(class ACharacterBase* NewCharacterRef);

	UFUNCTION()
		void UpdateProjectileStat(FProjectileStatStruct NewStat);

	UFUNCTION()
		void OnProjectileBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
			, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnTargetBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
			, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void ActivateProjectileMovement();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ACharacterBase* GetOwnerCharacterRef() { return OwnerCharacterRef; }

//------ private ������Ƽ ------------------
private: 
	UPROPERTY()
		bool bIsActivated = false;

	UPROPERTY()
		AController* SpawnInstigator;

	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;
};
