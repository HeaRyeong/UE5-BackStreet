// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "CollisionQueryParams.h"
#include "WeaponStatStructBase.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class BACKSTREET_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//------ Global -------------------
public:
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		UParticleSystem* HitEffectParticle;

	//Weapon의 종합 Stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		FWeaponStatStruct WeaponStat;

	//공격 처리
	UFUNCTION()
		void Attack();

	//공격 마무리 처리
	UFUNCTION()
		void StopAttack();

	//Weapon Stat 초기화
	UFUNCTION(BlueprintCallable)
		void InitWeaponStat(bool WeaponType, FWeaponStatStruct NewStat);

//------ Projectile 관련-------------
public:
	//발사체를 생성
	UFUNCTION()
		class AProjectileBase* CreateProjectile();

	//발사체 초기화
	UFUNCTION()
		void FireProjectile(); 

protected:
	//SoftObjRef로 대체 예정
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Weapon")
		TSubclassOf<class AProjectileBase> ProjectileClass;

//-------- Melee 관련 ------------
public:
	//Linetrace를 통한 근접 공격
	UFUNCTION()
		void MeleeAttack();

	//현재 Combo 수를 반환 
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentMeleeComboCnt() { return MeleeComboCnt; }

	//Melee Combo 초기화
	UFUNCTION()
		void ResetMeleeCombo();

protected:
	//근접 공격 Interval
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Setup")
		float MeleeAtkInterval = 0.5f;

	//현재 MeleeCombo 수
	UPROPERTY(BlueprintReadOnly)
		int32 MeleeComboCnt = 0;

private:
	//캐릭터 Ref
	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;

	UPROPERTY()
		FTimerHandle MeleeAtkTimerHandle;

	UPROPERTY()
		FTimerHandle MeleeComboTimerHandle;

	UPROPERTY()
		float MeleeAtkComboRemainTime = 1.0f;
};
