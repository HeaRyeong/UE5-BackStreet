// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

DECLARE_DELEGATE_OneParam(FDelegateEnemyDeath, class AEnemyCharacterBase*);
DECLARE_DELEGATE_OneParam(FDelegateEnemyDamage, class AActor*);

UCLASS()
class BACKSTREET_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

// ------ Global, Component ------------
public:
	AEnemyCharacterBase();
	
	//적 Death 이벤트
	FDelegateEnemyDeath EnemyDeathDelegate;

	//적 Hit 이벤트
	FDelegateEnemyDamage EnemyDamageDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|UI")
		class UWidgetComponent* FloatingHpBar;

// ----- 기본 프로퍼티 -----------
public:
	// 소속 타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* TileRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
		int32 EnemyID;

	//적이 최초로 소유하는 무기의 ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		int32 DefaultWeaponID;

// ----- Action ---------------
public:
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
		virtual void TryAttack() override;

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
		virtual void StopAttack() override;

	UFUNCTION()
		virtual void Die() override;

	UFUNCTION(BlueprintCallable)
		void Turn(float Angle);

	//플레이어를 발견한 직후 취할 Action(Anim), 재생 시간을 반환
	UFUNCTION(BlueprintCallable)
		float PlayPreChaseAnimation();

// ----- 캐릭터 스탯 및 상태 관련 ---------
public:
	UFUNCTION(BlueprintCallable)
		void InitEnemyStat();

	UFUNCTION(BlueprintCallable)
		virtual	void AddNewBuffDebuff(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer = nullptr, float TotalTime = 0.0f, float Value = 0.0f);

protected:
	UFUNCTION()
		void SetDefaultWeapon();

	UFUNCTION()
		void SetDefaultStat();

// ---- 그 외 (위젯, 사운드 등) ----
public:
	UFUNCTION(BlueprintImplementableEvent)
		void InitFloatingHpWidget();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		USoundCue* HitImpactSound;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* PreChaseAnimMontage;
};
