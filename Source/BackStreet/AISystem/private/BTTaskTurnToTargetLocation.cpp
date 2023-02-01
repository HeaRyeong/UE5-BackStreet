// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/BTTaskTurnToTargetLocation.h"
#include "../../Character/public/EnemyCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

#define RANGED_WEAPON_ERROR_TOLERANCE 0.2f
#define MELEE_WEAPON_ERROR_TOLERANCE 0.5f;

typedef UKismetMathLibrary UKML;


UBTTaskTurnToTargetLocation::UBTTaskTurnToTargetLocation()
{
    NodeName = "BTT_TurnToTarget";
}

void UBTTaskTurnToTargetLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    UBlackboardData* bbAsset = GetBlackboardAsset();
    if (bbAsset != nullptr)
    {
        TargetCharacterBBKey.ResolveSelectedKey(*bbAsset);
        TargetLocationBBKey.ResolveSelectedKey(*bbAsset);
    }
    else
    {
        UE_LOG(LogBehaviorTree, Warning, TEXT("BTTask_%s �ʱ�ȭ ����."), *GetName());
    }
}

EBTNodeResult::Type UBTTaskTurnToTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    OwnerCharacterRef = Cast<AEnemyCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    BlackboardComponentRef = OwnerComp.GetBlackboardComponent();
    
    InitTargetLocationFromBBKey();
    
    const float turnAngle = CalculateTurnAngle(OwnerCharacterRef->GetActorLocation()
                                            , OwnerCharacterRef->GetCapsuleComponent()->GetForwardVector());
    if (turnAngle != 0.0f)
    {
        OwnerCharacterRef->Turn(turnAngle);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}

void UBTTaskTurnToTargetLocation::InitTargetLocationFromBBKey()
{
    FName bbKeyName = TargetCharacterBBKey.SelectedKeyName;
    const AActor* targetCharacterRef = Cast<AActor>(BlackboardComponentRef->GetValueAsObject(bbKeyName));

    if (IsValid(targetCharacterRef))
    {
        TargetLocation = targetCharacterRef->GetActorLocation();
    }
    else
    {
        bbKeyName = TargetLocationBBKey.SelectedKeyName;
        TargetLocation = BlackboardComponentRef->GetValueAsVector(bbKeyName);
    }
}

float UBTTaskTurnToTargetLocation::CalculateTurnAngle(const FVector& OwnerLocation, const FVector& OwnerForwardVector)
{
    FVector2D pawnLocation2D = UKML::Conv_VectorToVector2D(OwnerLocation);
    FVector2D targetLocation2D = UKML::Conv_VectorToVector2D(TargetLocation);
    FVector2D pawnForwardVector2D = UKML::Conv_VectorToVector2D(OwnerForwardVector);

    FVector directionToTarget = (TargetLocation - OwnerLocation).GetSafeNormal();
    FVector2D directionToTarget2D = (targetLocation2D - pawnLocation2D).GetSafeNormal();
    pawnForwardVector2D = UKML::Normal2D(pawnForwardVector2D);

    float rotateAngle = 0.0f;
    const bool bHasRangedWeapon = BlackboardComponentRef->GetValueAsBool(FName("HasRangedWeapon"));
   
    /*DrawDebugLine(GetWorld(), OwnerLocation, OwnerLocation + OwnerForwardVector * 100.0f, FColor::Yellow, false, 5.0f, 1.0f, 20.0f);
    DrawDebugLine(GetWorld(), OwnerLocation, OwnerLocation + directionToTarget * 100.0f, FColor::Red, false, 5.0f, 1.0f, 20.0f);
    LogMessage(FString("pawn location : "), pawnLocation2D);
    LogMessage(FString("target location : "), targetLocation2D);
    LogMessage(FString("> pawn forward : "), pawnForwardVector2D);
    LogMessage(FString("> to target direction : "), directionToTarget2D);*/

    float errorTolerance = bHasRangedWeapon ? RANGED_WEAPON_ERROR_TOLERANCE : MELEE_WEAPON_ERROR_TOLERANCE;
   
    //Ÿ���� ���� ����� �÷��̾��� ������ �������� ������ ��ġ���� �ʴٸ�?
    if (!UKML::EqualEqual_Vector2DVector2D(directionToTarget2D - pawnForwardVector2D, FVector2D(0.0f), errorTolerance))
    {
        //�� ���� ������ ȸ������ ������
        rotateAngle = pawnForwardVector2D.Y * directionToTarget2D.X - pawnForwardVector2D.X * directionToTarget2D.Y;
        rotateAngle = rotateAngle / (UKML::VSize2D(pawnForwardVector2D) * UKML::VSize2D(directionToTarget2D));
        rotateAngle = UKML::DegAsin(rotateAngle);
        rotateAngle = FMath::Sign(rotateAngle) * FMath::Min(FMath::Abs(rotateAngle)
                                                            , UKML::RandomFloatInRange(10.0f, 30.0f));
        rotateAngle *= -1;
    }  
    return rotateAngle; //������ ��ġ�ϸ� Turn�� ������ �ʿ䰡 ����
}

void UBTTaskTurnToTargetLocation::LogMessage(FString str, FVector2D vec)
{
    UE_LOG(LogTemp, Warning, TEXT("%s) - {%.2lf, %.2lf}"), *str, vec.X, vec.Y);
}
