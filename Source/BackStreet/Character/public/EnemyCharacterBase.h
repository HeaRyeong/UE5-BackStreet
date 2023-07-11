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
	
	//�� Death �̺�Ʈ
	FDelegateEnemyDeath EnemyDeathDelegate;

	//�� Hit �̺�Ʈ
	FDelegateEnemyDamage EnemyDamageDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|UI")
		class UWidgetComponent* FloatingHpBar;

// ----- �⺻ ������Ƽ -----------
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
		int32 EnemyID;

	//���� ���ʷ� �����ϴ� ������ ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		int32 DefaultWeaponID;

	//�ִ� ������ �������� ����. �̼� �������� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|DropItem", meta = (UIMin = 0, UIMax = 2))
		int32 MaxSpawnItemCount;

	//���� �װ� ������ �������� Type ����Ʈ (�� �������� Idx�� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|DropItem")
		TArray<EItemCategoryInfo> SpawnItemTypeList;

	//���� �ױ� ������ ������ ID ����Ʈ (�� �������� Idx�� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|DropItem")
		TArray<uint8> SpawnItemIDList;

	//���� �װ� ������ �������� ���� Ȯ�� ����Ʈ  (0.0f ~ 1.0f),  (�� �������� Idx�� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|DropItem")
		TArray<float> ItemSpawnProbabilityList;

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

	//�÷��̾ �߰��� ���� ���� Action(Anim), ��� �ð��� ��ȯ
	UFUNCTION(BlueprintCallable)
		float PlayPreChaseAnimation();

// ----- ĳ���� ���� �� ���� ���� ---------
public:
	UFUNCTION(BlueprintCallable)
		void InitEnemyStat();

protected:
	UFUNCTION()
		void SetDefaultWeapon();

	UFUNCTION()
		void SetDefaultStat();

	UFUNCTION()
		void SpawnDeathItems();

// ---- VFX ---------------------
public:
	UFUNCTION(BlueprintCallable)
		void SetFacialMaterialEffect(bool NewState);

	//�׽�Ʈ �ڵ� -> ���ȭ ����
	UPROPERTY(EditDefaultsOnly)
		float knockBackStrength = 100000.0f;

// ---- �� �� (����, ���� ��) ----
public:
	UFUNCTION(BlueprintImplementableEvent)
		void InitFloatingHpWidget();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		USoundCue* HitImpactSound;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* PreChaseAnimMontage;

public:
	void ClearAllTimerHandle();

private:
	//���� Turn�� ������ �ʰ� TimeOut ó�� �����ִ� Ÿ�̸� �ڵ�
	UPROPERTY()
		FTimerHandle TurnTimeOutTimerHandle;
};
