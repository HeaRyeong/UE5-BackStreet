// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/AIControllerBase.h"
#include "../../Global/public/BackStreetGameModeBase.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

AAIControllerBase::AAIControllerBase()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_PERCEPTION"));

	InitAIPerceptionSystem();
}

void AAIControllerBase::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	//���� ��, AI ������ ��� ���� �д�.
	if (IsValid(GetBrainComponent()))
	{
		GetBrainComponent()->PauseLogic(FString("PrevGameStart"));
	}
	Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode())->ClearResourceDelegate.AddDynamic(this, &AAIControllerBase::DeactivateAI);
	Cast<ABackStreetGameModeBase>(GetWorld()->GetAuthGameMode())->StartChapterDelegate.AddDynamic(this, &AAIControllerBase::ActivateAI);
}

void AAIControllerBase::ActivateAI()
{
	GetBrainComponent()->ResumeLogic(FString("GameStart")); //Delegate�� ���� Chapter�� �ʱ�ȭ �Ǹ�, Activate �Ѵ�.
	UE_LOG(LogTemp, Warning, TEXT("ActivateAI"));
}

void AAIControllerBase::DeactivateAI()
{
	GetBrainComponent()->StopLogic(FString("GameOver"));
}

void AAIControllerBase::InitAIPerceptionSystem()
{
	SightPerceptionConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_CONFIG"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightPerceptionConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightPerceptionConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightPerceptionConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightPerceptionConfig->SetMaxAge(MaxSightAge);
	SightPerceptionConfig->SightRadius = SightRadius;

	AIPerceptionComponent->SetDominantSense(*SightPerceptionConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightPerceptionConfig);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::UpdateTargetPerception);
}

void AAIControllerBase::UpdateTargetPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (IsValid(Actor) && Actor->ActorHasTag("Player") && Stimulus.WasSuccessfullySensed())
	{
		GetWorldTimerManager().ClearTimer(SightLossTimerHandle);
		GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", true);
		GetBlackboardComponent()->SetValueAsObject("TargetCharacter", Actor);
	}
	else
	{
		GetWorldTimerManager().SetTimer(SightLossTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", false);
			GetBlackboardComponent()->SetValueAsObject("TargetCharacter", nullptr);
		}), 1.0f, false, 4.0f);
	}
	
}

void AAIControllerBase::ClearAllTimerHandle()
{
	GetWorldTimerManager().ClearTimer(SightLossTimerHandle);
}