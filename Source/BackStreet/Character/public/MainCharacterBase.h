// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "CharacterBase.h"
#include "GameFramework/Character.h"
#include "MainCharacterBase.generated.h"

UCLASS()
class BACKSTREET_API AMainCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ------- ������Ʈ ----------
public:
	//�÷��̾� ���� ī�޶� ��
	UPROPERTY(VisibleDefaultsOnly)
		USpringArmComponent* CameraBoom;

	//�÷��̾��� ���� ī�޶�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* FollowingCamera;

// ------- Character Action ------- 
public:
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void Roll();

	UFUNCTION()
		virtual void TryReload() override;

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

	//Rotation ���� ����� Ŀ�� ��ġ�� �Ѵ�
	UFUNCTION()
		void RotateToCursor();

	//Rotation ���� ����� �⺻ ����� Movement �������� �ǵ�����
	UFUNCTION(BlueprintCallable)
		void ResetRotationToMovement();

	UFUNCTION()
		virtual void SwitchToNextWeapon() override;

	UFUNCTION()
		virtual void DropWeapon() override;

// ------- ���� / ����� ---------------
public: 
	//���� or ����� ���¸� ����
	UFUNCTION(BlueprintCallable)
		virtual	bool SetBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f) override;

	//���� or ����� ���¸� �ʱ�ȭ�Ѵ�
	UFUNCTION(BlueprintCallable)
		virtual void ResetStatBuffDebuffState(bool bIsDebuff, uint8 BuffDebuffType, float ResetVal) override;

	//Ư�� Debuff�� Ÿ�̸Ӹ� �����Ѵ�.
	UFUNCTION(BlueprintCallable)
		virtual void ClearBuffDebuffTimer(bool bIsDebuff, uint8 BuffDebuffType) override;

	//��� Buff/Debuff�� Ÿ�̸Ӹ� ����
	UFUNCTION(BlueprintCallable)
		virtual void ClearAllBuffDebuffTimer(bool bIsDebuff) override;

// -------- VFX -----------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		class UNiagaraComponent* BuffNiagaraEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		class UNiagaraComponent* DirectionNiagaraEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		TArray<class UNiagaraSystem*> BuffNiagaraEffectList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		TArray<class UNiagaraSystem*> DebuffNiagaraEffectList;

	UFUNCTION()
		void DeactivateBuffNiagara();

// ------- �� �� -----------
public:
	//UFUNCTION()
	virtual void ClearAllTimerHandle() override;

private:
	UPROPERTY()
		class AMainCharacterController* PlayerControllerRef;

	//���� ��, ���콺 Ŀ���� ��ġ�� ĳ���Ͱ� �ٶ󺸴� ������ �ʱ�ȭ�ϴ� Ÿ�̸�
	//�ʱ�ȭ �ÿ��� �ٽ� Movement �������� ĳ������ Rotation�� Set
	UPROPERTY()
		FTimerHandle RotationResetTimerHandle;

	//���� �ݺ� �۾� Ÿ�̸�
	UPROPERTY()
		FTimerHandle AttackLoopTimerHandle;

	//������ ������ Ÿ�̸�
	UPROPERTY()
		FTimerHandle RollTimerHandle;
};
