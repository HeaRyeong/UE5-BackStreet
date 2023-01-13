// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "CharacterInfoStructBase.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
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
		void TakeHeal(float HealAmount, bool bIsTimerEvent = false, uint8 BuffType = 0);

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
	//UFUNCTION()
		//void ChangeWeapon();

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
	virtual	bool SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);
	
	//���� or ����� ���¸� �ʱ�ȭ�Ѵ�
	virtual void ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal);

	//Ư�� Buff/Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	//bForceClear : ������ Ÿ�̸Ӹ� �����Ѵ�. �׷��� ������ RemainingTime�� 0.0f�� �����.
	virtual void ClearBuffTimer(bool bIsDebuff, uint8 BuffType);
	
	//��� Buff/Debuff�� Ÿ�̸Ӹ� ����
	virtual void ClearAllBuffTimer(bool bIsDebuff);

	//������� Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//������ Ȱ��ȭ �Ǿ��ִ��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetBuffIsActive(ECharacterBuffType BuffType);

	//����/����� ���� �ð��� ��ȯ
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetBuffRemainingTime(bool bIsDebuff, uint8 BuffType);

	//���� / ����� Ÿ�̸� �ڵ��� �����ڸ� ��ȯ
	UFUNCTION()
		FTimerHandle& GetBuffTimerHandleRef(bool bIsDebuff, uint8 BuffType);

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

	//���� �� ������ �ڵ�
	UPROPERTY()
		FTimerHandle AtkIntervalHandle;

	UPROPERTY()
		FTimerHandle ReloadTimerHandle;
};
