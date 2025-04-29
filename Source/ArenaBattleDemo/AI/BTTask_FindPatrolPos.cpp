// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"

#include "ABAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    // ���̰��̼� �ý����� ����� �������� �̵� ������ ��ġ�� ���� �ڿ�
    // �����忡 ���� ��ġ�� ����.

    // �����̺��Ʈ���� �����ϴ� ��Ʈ�ѷ��� �����ϴ� �� ���� ��������.
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }

    // ������̼� �ý����� ����ϱ� ���� ������ ��������.
    UNavigationSystemV1* NavSystem
        = UNavigationSystemV1::GetNavigationSystem(
            ControllingPawn->GetWorld()
        );

    // ��ȿ���� �˻�.
    if (!NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    // �������̽��� ����ȯ.
    IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);

    // ��ȯ�� �����ϸ� ���� ��ȯ.
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    // ���� ������ �ʱ� ��ġ ��������.
    FVector Origin
        = OwnerComp.GetBlackboardComponent()->GetValueAsVector(
            BBKEY_HOMEPOS
        );

    // AIPawn���κ��� ���� �ݰ� �޾ƿ���.
    float PatrolRadius = AIPawn->GetAIPatrolRadius();

    // ��� ������ ���� ����.
    FNavLocation NextPatrolPos;
    if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
    {
        // ���� ��ġ�� �� �����Ǹ�, �����忡 ����.
        //OwnerComp.GetBlackboardComponent()->SetValueAsVector(
        //    BBKEY_PATROLPOS, NextPatrolPos
        //);

        OwnerComp.GetBlackboardComponent()->SetValueAsVector(
            BBKEY_PATROLPOS, NextPatrolPos.Location
        );

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}