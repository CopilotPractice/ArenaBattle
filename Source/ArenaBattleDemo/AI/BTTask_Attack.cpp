// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"


UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AIController�� �����ϴ� ��
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);

	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// ĳ���Ͱ� ������ ������ �� ������ ��������Ʈ ����
	FAICharacterAttackFinished OnAttackFinished;

	//���� �Լ��� ����Ͽ� �������̺v�� ���ε�
	//ȣ���ϱ� ���� ����
	OnAttackFinished.BindLambda(
		[&]() // &�� ��� ������ ĸ�ĵǾ OwnerComp�� ������ �� ����
		{
		
			// ĳ������ ������ ���� �� �½�ũ�� ������ ���� ����
			// InProgress�� ��ȯ�� �� �½�ũ�� ���� ������ �Ҷ���
			//FinishLatentTask �Լ��� ��� ����
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	// ĳ���Ͱ� ������ ������ �� ������ ��������Ʈ ����(����)
	AIPawn->SetAIAttackDelegate(OnAttackFinished); 

	// ���� ��� ����
	AIPawn->AttackByAI(); // ���� ����� ������ AI �� ���� Animation ����
						  // ������ ó�� ��� �ð��� �ʿ�

	// ���� ��� ���� �� ��ٷ� ����� �� �� ���� ������,
	// ������(InProgress) ���� ��ȯ
	return EBTNodeResult::InProgress;
}
