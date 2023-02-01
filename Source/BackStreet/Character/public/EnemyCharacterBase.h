// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "CharacterBase.h"
#include "EnemyCharacterBase.generated.h"

DECLARE_DELEGATE_OneParam(FDelegateEnemyDeath, class AEnemyCharacterBase*);

UCLASS()
class BACKSTREET_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacterBase();
	
	//적 Death 이벤트
	FDelegateEnemyDeath EnemyDeathDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ----- 기본 함수들 ------
public:
	// 소속 타일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATileBase* TileRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
		int32 EnemyID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		int32 DefaultWeaponID;

public:
	UFUNCTION(BlueprintCallable)
		void InitEnemyStat();

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

// ---- 적 캐릭터 Ation ----
	UFUNCTION(BlueprintCallable)
		void Turn(float Angle);

// ---- 디버프 / 버프 -----
	//버프 or 디버프 상태를 지정
	UFUNCTION(BlueprintCallable)
		virtual	bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f) override;

	//버프 or 디버프 상태를 초기화한다
	UFUNCTION(BlueprintCallable)
		virtual void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal) override;
};
