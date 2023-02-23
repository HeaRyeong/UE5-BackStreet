// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Global/public/BackStreet.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTaskTurnToTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API UBTTaskTurnToTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskTurnToTargetLocation();

	UPROPERTY(EditInstanceOnly)
		FBlackboardKeySelector TargetCharacterBBKey;

	UPROPERTY(EditInstanceOnly)
		FBlackboardKeySelector TargetLocationBBKey;

protected:
	//BT�� �����尡 ������ �Ǿ����� ���� ��츦 ����
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	//TargetCharacter�� Valid��� TargetCharacter�� WorldLocation�� Target�� ����
	UFUNCTION()
		void InitTargetLocationFromBBKey();
	
	//�̹� tick�� �� Angle�� ��ȯ
	UFUNCTION()
		double CalculateTurnAngle(const FVector& OwnerLocation, const FVector& OwnerForwardVector);

	UFUNCTION()
		void LogMessage(FString str, FVector2D vec);

private:
	UPROPERTY()
		class AEnemyCharacterBase* OwnerCharacterRef;

	UPROPERTY()
		FVector TargetLocation;

	UPROPERTY()
		class UBlackboardComponent* BlackboardComponentRef;
};
