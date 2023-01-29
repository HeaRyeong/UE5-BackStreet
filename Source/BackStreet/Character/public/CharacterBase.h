// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

#define InventoryMaxSize 6

DECLARE_DELEGATE_OneParam(FEnemyDieDelegate, class ACharacterBase*);

UCLASS()
class BACKSTREET_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

//----- �⺻ �Լ� ----------
public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ------- ĳ���� ������Ʈ -------------
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "WeaponInventory")
		UChildActorComponent* WeaponActor;

// ------- Character Action �⺻ ------- 
public:
	//Input�� Binding �Ǿ� ������ �õ� (AnimMontage�� ȣ��)
	virtual void TryAttack();

	//AnimNotify�� Binding �Ǿ� ���� ������ ����
	virtual void Attack();

	virtual void StopAttack();

	virtual void TryReload(); 
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
		, AController* EventInstigator, AActor* DamageCauser) override;

	//�÷��̾ ���� �ش� Action�� �����ϰ� �ִ��� ��ȯ
	UFUNCTION(BlueprintCallable)
		bool GetIsActionActive(ECharacterActionType Type) { return CharacterState.CharacterActionState == Type; }
	
	//�÷��̾��� ActionState�� Idle�� ��ȯ�Ѵ�.
	UFUNCTION(BlueprintCallable)
		void ResetActionState();
		
	//����� �������� ���� (��ȸ��)
	UFUNCTION()
		float TakeDebuffDamage(float DamageAmount, uint8 DebuffType, AActor* Causer);

	//�÷��̾ ü���� ȸ���� (��ȸ��)
	UFUNCTION()
		void TakeHeal(float HealAmount, bool bIsTimerEvent = false, uint8 BuffDebuffType = 0);

	UFUNCTION()
		void Die();

// ------- Character Stat/State ------- 
public:
	//ĳ������ ���� ������ �ʱ�ȭ
	UFUNCTION()
		void InitCharacterState();

	//ĳ������ ������ ������Ʈ
	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FCharacterStatStruct NewStat);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FCharacterStatStruct GetCharacterStat() { return CharacterStat; }

// ------ ���� ���� ----------------
public:
	UFUNCTION(BlueprintCallable)
		void ChangeWeapon(AWeaponBase* newWeaponClass);

	//���� ���� ������ �ʱ�ȭ
	UFUNCTION()
		void InitWeapon();

	//���� Ref�� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		class AWeaponBase* GetWeaponActorRef();

	//����Action ������ Interval�� �����ϴ� Ÿ�̸Ӹ� ����
	UFUNCTION()
		void ResetAtkIntervalTimer();

// ------ ĳ���� ���� / ����� ---------------
public:
	//������ ������� �Ǵ�
	virtual	bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);
	
	//���� or ����� ���¸� �ʱ�ȭ�Ѵ�
	virtual void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal);

	//Ư�� Buff/Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	virtual void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType);
	
	//��� Buff/Debuff�� Ÿ�̸Ӹ� ����
	virtual void ClearAllBuffDebuffTimer(bool bIsDebuff);

	UFUNCTION()
		bool SetBuffTimer(ECharacterBuffType BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	UFUNCTION()
		bool SetDebuffTimer(ECharacterDebuffType DebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//������� Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//������ Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetBuffIsActive(ECharacterBuffType BuffType);

	//����/����� ���� �ð��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetBuffRemainingTime(bool bIsDebuff, uint8 BuffDebuffType);

	//���� / ����� Ÿ�̸� �ڵ��� �����ڸ� ��ȯ
	UFUNCTION()
		FTimerHandle& GetBuffDebuffTimerHandleRef(bool bIsDebuff, uint8 BuffDebuffType);

// ----- ĳ���� �ִϸ��̼� -------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		TArray<UAnimMontage*> AttackAnimMontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* RollAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* ReloadAnimMontage;

// ------ �� �� ĳ���� ������Ƽ / �Լ� ---------------
protected:
	UFUNCTION()
		void InitGamemodeRef();

	//UFUNCTION()
		virtual void ClearAllTimerHandle();

	//ĳ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStatStruct CharacterStat;

	//ĳ������ ���� ����
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
		FCharacterStateStruct CharacterState;

	UPROPERTY()
		class ABackStreetGameModeBase* GamemodeRef;

private:
	UPROPERTY()
		TArray<FTimerHandle> BuffDebuffTimerHandleList;

private:
	//���� �� ������ �ڵ�
	UPROPERTY()
		FTimerHandle AtkIntervalHandle;

	UPROPERTY()
		FTimerHandle ReloadTimerHandle;

	// -------- �� �κ� ���� ó�� ���� ��������Ʈ --------

public:
	FEnemyDieDelegate FDieDelegate;

	// ���� �κ��丮 ����

public:
	UFUNCTION(BlueprintCallable)
	void InitWeaponInventory();
	UFUNCTION(BlueprintCallable)
	void AddWeaponInventory(AWeaponBase* Weapon);
	UFUNCTION(BlueprintCallable)
	void SwitchWeapon();

public:
	UPROPERTY(EditAnywhere, Category = "WeaponInventory")
		TArray<AWeaponBase*> WeaponInventory;
	UPROPERTY(EditAnywhere, Category = "WeaponInventory")
		TArray<int32> WeaponIDInventory;
	UPROPERTY(EditAnywhere, Category = "WeaponInventory")
		int8 InventorySize;
	UPROPERTY(EditAnywhere, Category = "WeaponInventory")
		int8 InventoryIdx;

};
