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

//-------- Global -----------------
public:
	AMainCharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

protected:
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
protected: 
	//���� or ����� ���¸� ����
	UFUNCTION(BlueprintCallable)
		virtual	void AddNewBuffDebuff(bool bIsDebuff, uint8 BuffDebuffType, AActor* Causer = nullptr, float TotalTime = 0.0f, float Value = 0.0f);

// -------- VFX -----------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		class UNiagaraComponent* BuffNiagaraEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		class UNiagaraComponent* DirectionNiagaraEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		TArray<class UNiagaraSystem*> BuffNiagaraEffectList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|VFX")
		TArray<class UNiagaraSystem*> DebuffNiagaraEffectList;

	UFUNCTION()
		void ActivateBuffNiagara(bool bIsDebuff, uint8 BuffDebuffType);

	UFUNCTION()
		void DeactivateBuffEffect();

// -------- Sound ----------------
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* RollSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* WalkSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* BuffSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* DebuffSound;

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
