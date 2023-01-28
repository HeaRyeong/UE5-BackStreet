// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/BTStateManageServiceBase.h"
#include "../../Character/public/CharacterBase.h"
#include "../../Item/public/WeaponBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#define INF 1e7f

UBTStateManageServiceBase::UBTStateManageServiceBase(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("UpdateAIState");
	RandomDeviation = 0.1f;
	Interval = 0.4f;

	bCreateNodeInstance = true;
	AIBehaviorState = EAIBehaviorType::E_Patrol;
}

void UBTStateManageServiceBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsValid(OwnerCharacterRef)) //���� ���� : BB�� ĳ���� Ref�� �ʱ�ȭ
	{
		OwnerCharacterRef = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
		BlackboardRef = OwnerComp.GetBlackboardComponent();
		BlackboardRef->SetValueAsVector(FName("SpawnLocation"), OwnerCharacterRef->GetActorLocation());
	}
	else 
	{
		UpdateAIState();

		const FVector& spawnLocation = BlackboardRef->GetValueAsVector(FName("SpawnLocation"));
		BlackboardRef->SetValueAsEnum("AIBehaviorState", (uint8)AIBehaviorState);
	}
}

void UBTStateManageServiceBase::UpdateAIState()
{
	if (CheckReturnState())
	{
		if (CheckPatrolState())
		{
			AIBehaviorState = EAIBehaviorType::E_Patrol;
		}
		else
		{
			AIBehaviorState = EAIBehaviorType::E_Return;
		}
	}
	else if (CheckChaseState())
	{
		if (CheckAttackState())
		{
			AIBehaviorState = EAIBehaviorType::E_Attack;
		}
		else
		{
			AIBehaviorState = EAIBehaviorType::E_Chase;
		}
	}
	else
	{
		AIBehaviorState = EAIBehaviorType::E_Patrol;
	}
}

bool UBTStateManageServiceBase::CheckPatrolState()
{
	const FVector& spawnLocation = BlackboardRef->GetValueAsVector(FName("SpawnLocation"));
	if (GetDistanceTo(spawnLocation) <= 100.0f) return true;
	return false;
}

bool UBTStateManageServiceBase::CheckReturnState()
{
	const FVector& spawnLocation = BlackboardRef->GetValueAsVector(FName("SpawnLocation"));
	if (AIBehaviorState == EAIBehaviorType::E_Return || GetDistanceTo(spawnLocation) >= 1000.0f) return true;
	return false;
}

bool UBTStateManageServiceBase::CheckChaseState()
{
	return BlackboardRef->GetValueAsBool("HasLineOfSight");
}

bool UBTStateManageServiceBase::CheckAttackState()
{
	const AActor* targetCharacterRef = Cast<AActor>(BlackboardRef->GetValueAsObject(FName("TargetCharacter")));
	float distanceToTarget = GetDistanceTo(targetCharacterRef->GetActorLocation());
	return distanceToTarget <= (OwnerCharacterRef->GetWeaponActorRef())->GetAttackRange();
}

float UBTStateManageServiceBase::GetDistanceTo(const FVector& EndLocation)
{
	return UKismetMathLibrary::Vector_Distance(OwnerCharacterRef->GetActorLocation(), EndLocation);
}