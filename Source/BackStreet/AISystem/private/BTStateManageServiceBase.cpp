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
	RandomDeviation = 0.0f;
	Interval = 0.1f;

	bNotifyBecomeRelevant = true; //OnBecomeRelevant 이벤트를 OnReceieveAiTick 처럼 사용하기 위함	
}

void UBTStateManageServiceBase::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!IsValid(OwnerCharacterRef))
	{
		OwnerCharacterRef = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
		BlackboardRef = OwnerComp.GetBlackboardComponent();
		BlackboardRef->SetValueAsVector(FName("SpawnPoint"), OwnerCharacterRef->GetActorLocation());
	}
	else
	{
		UpdateAIState();
		BlackboardRef->SetValueAsEnum("AIBehaviorState", (uint8)AIBehaviorState);
	}
}

void UBTStateManageServiceBase::UpdateAIState()
{
	if (CheckReturnState())
	{
		AIBehaviorState = EAIBehaviorType::E_Return;
	}
	else if (CheckChaseState())
	{
		AIBehaviorState = EAIBehaviorType::E_Chase;
		if (CheckAttackState())
		{
			AIBehaviorState = EAIBehaviorType::E_Attack;
		}
	}
	else
	{
		AIBehaviorState = EAIBehaviorType::E_Patrol;
	}

}

bool UBTStateManageServiceBase::CheckReturnState()
{
	if (!IsValid(OwnerCharacterRef)) return false;
	if (AIBehaviorState == EAIBehaviorType::E_Return) return true;

	const FVector& spawnLocation = BlackboardRef->GetValueAsVector(FName("SpawnLocation"));
	return GetDistanceTo(spawnLocation) >= 1000.0f;
}

bool UBTStateManageServiceBase::CheckChaseState()
{
	if (!IsValid(OwnerCharacterRef)) return false;
	return BlackboardRef->GetValueAsBool("HasLineOfSight");
}

bool UBTStateManageServiceBase::CheckAttackState()
{
	if (!IsValid(OwnerCharacterRef)) return false;
	
	const AActor* targetCharacterRef = Cast<AActor>(BlackboardRef->GetValueAsObject(FName("TargetCharacter")));
	float distanceToTarget = GetDistanceTo(targetCharacterRef->GetActorLocation());
	return distanceToTarget >= (OwnerCharacterRef->GetWeaponActorRef())->GetAttackRange();
}

float UBTStateManageServiceBase::GetDistanceTo(const FVector& EndLocation)
{
	if (!IsValid(OwnerCharacterRef)) return INF;
	return UKismetMathLibrary::Vector_Distance(OwnerCharacterRef->GetActorLocation(), EndLocation);
}