// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"

void AAIControllerBase::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
