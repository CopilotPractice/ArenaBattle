// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAI.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	// �����Ϳ��� ���� ��� �̸� ����.
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AIController�� �����ϴ� ��.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	// ���� ������ ����.
	if (!ControllingPawn)
	{
		return false;
	}

	// �������̽��� ����ȯ.
	IABCharacterAIInterface* AIPawn 
		= Cast<IABCharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return false;
	}

	// �����忡 ����� ĳ���� ��.
	APawn* Target 
		= Cast<APawn>(
			OwnerComp.GetBlackboardComponent()->GetValueAsObject(
				BBKEY_TARGET
			)
		);

	// �����忡 ����� ���� ������ ����.
	if (!Target)
	{
		return false;
	}

	// ĳ���Ϳ��� �Ÿ� ���.
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	// ���� ���� ��.
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();

	// ĳ���Ϳ��� �Ÿ��� ���� ���ɰŸ����� ������� Ȯ�� �� ��� ��ȯ.
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}