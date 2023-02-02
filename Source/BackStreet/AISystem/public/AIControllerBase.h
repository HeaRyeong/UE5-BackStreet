// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

//-- �⺻ �Լ� �� ������Ʈ ---------------
public:
	AAIControllerBase();

	UFUNCTION()
		virtual void OnPossess(APawn* PossessedPawn) override;

	UFUNCTION()
		virtual void BeginPlay() override;

	UFUNCTION()
		void DeactivateAI();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|AI")
		class UAIPerceptionComponent* AIPerceptionComponent;

//--Perception ���� ---------------------
protected:
	//�þ� ����
	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
		float SightRadius;

	//�ִ� �þ� ����
	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
		float MaxSightAge;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
		class UAISenseConfig_Sight* SightPerceptionConfig;

public:
	UFUNCTION()
		void InitAIPerceptionSystem();

	UFUNCTION()
		void UpdateTargetPerception(AActor* Actor, FAIStimulus Stimulus);

//--�ڿ� ���� ---------------------
public:
	//���� ���� �̺�Ʈ�� ���ε��ؼ� ���
	UFUNCTION()
		void ClearAllTimerHandle();

//--��Ÿ Property-----------------------
protected:
	//������ BT�� ����
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;

	//AI�� ���� ������ �ִ� BT �׼� ���¸� ��Ÿ��
	UPROPERTY(BlueprintReadWrite)
		EAIBehaviorType AIBehaviorState;

private:
	//ĳ���� ���� Time-Out ��, ����� ����� ������ ����
	//�ش� �Լ��� ���ε��Ͽ� ����� Ÿ�̸� �ڵ�
	UPROPERTY()
		FTimerHandle SightLossTimerHandle;
};
