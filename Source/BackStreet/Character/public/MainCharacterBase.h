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

	//테스트용 코드
	UFUNCTION(BlueprintCallable)
		void ActivateHealAbility();
	UFUNCTION(BlueprintCallable)
		void DeactivateHealAbility();
	// 임시 어빌리티 매니저 getter 함수
	UFUNCTION(BlueprintCallable)
		class UAbilityManagerBase* GetAbilityManager() {return AbilityManagerRef;}

protected:
	virtual void BeginPlay() override;

// ------- 컴포넌트 ----------
public:
	//플레이어 메인 카메라 붐
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* CameraBoom;

	//플레이어의 메인 카메라
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
		void ZoomIn(float Value);

	UFUNCTION()
		void TryInvestigate();

	UFUNCTION(BlueprintCallable)
		void Investigate(AActor* TargetActor);

	UFUNCTION()
		virtual void TryReload() override;

	UFUNCTION(BlueprintCallable)
		virtual void TryAttack() override;

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
		virtual void StopAttack() override;

	UFUNCTION()
		virtual void Die() override;

	//Rotation 조절 방식을 기본 방식인 Movement 방향으로 되돌린다
	UFUNCTION(BlueprintCallable)
		void ResetRotationToMovement();

	UFUNCTION()
		virtual void SwitchToNextWeapon() override;

	UFUNCTION()
		virtual void DropWeapon() override;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	//Rotation 조절 방식을 커서 위치로 한다
	UFUNCTION()
		void RotateToCursor();

	UFUNCTION()
		TArray<AActor*> GetNearInteractionActorList();

// ------- 버프 / 디버프 ---------------
protected: 
	//버프 or 디버프 상태를 지정
	UFUNCTION(BlueprintCallable)
		virtual	bool AddNewDebuff(ECharacterDebuffType BuffDebuffType, AActor* Causer = nullptr, float TotalTime = 0.0f, float Value = 0.0f);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Material")
		class UMaterialInterface* NormalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Material")
		class UMaterialInterface* WallThroughMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Material")
		TArray<class UTexture*> EmotionTextureList;

private:
	UPROPERTY()
		bool bIsWallThroughEffectActivated = false;

	UFUNCTION()
		void ActivateBuffNiagara(bool bIsDebuff, uint8 BuffDebuffType);

	UFUNCTION()
		void DeactivateBuffEffect();

	UFUNCTION()
		void UpdateWallThroughEffect();

	//캐릭터가 데미지를 입을 시, 빨간 Pulse 효과와 표정 텍스쳐 효과를 적용
	UFUNCTION()
		void SetFacialDamageEffect(bool NewState);

// -------- Asset ----------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* RollSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* ErrorSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* BuffSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Sound")
		class USoundCue* DebuffSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* InvestigateAnimation;

// ------- 그 외 -----------
public:
	UFUNCTION()
	virtual void ClearAllTimerHandle() override;

private:
	UPROPERTY()
		class UAbilityManagerBase* AbilityManagerRef;

	UPROPERTY()
		class AMainCharacterController* PlayerControllerRef;

	//공격 시, 마우스 커서의 위치로 캐릭터가 바라보는 로직을 초기화하는 타이머
	//초기화 시에는 다시 Movement 방향으로 캐릭터의 Rotation을 Set
	UPROPERTY()
		FTimerHandle RotationResetTimerHandle;

	//공격 반복 작업 타이머
	UPROPERTY()
		FTimerHandle AttackLoopTimerHandle;

	//구르기 딜레이 타이머
	UPROPERTY()
		FTimerHandle RollTimerHandle;

	//버프 나이아가라 이펙트 리셋 타이머
	UPROPERTY()
		FTimerHandle BuffEffectResetTimerHandle;

	//캐릭터 얼굴 효과 (머티리얼 값 변경) 리셋 타이머
	UPROPERTY()
		FTimerHandle FacialEffectResetTimerHandle;
};
