// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABAI.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// ��� �̸� ����.
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AiController�� �����ϴ� ��.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	//if (ControllingPawn == nullptr)
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// �÷��̾�.
	APawn* TargetPawn = Cast<APawn>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET)
	);

	if (!TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	// �������̽� ����ȯ.
	IABCharacterAIInterface* AIPawn
		= Cast<IABCharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// ȸ���� �ӵ� ��.
	float TurnSpeed = AIPawn->GetAITurnSpeed();

	// �ٶ� ���� ���ϱ�.
	FVector LookVector
		= TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// ���� ������ ����.
	LookVector.Z = 0.0f;

	// �ٶ� ������ �̿��� FRotator ���ϱ�.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// �ε巴�� ȸ�� ����.

	// ���� �����ӿ� ������ ȸ�� ��.
	FRotator CurrentRot = FMath::RInterpTo(
		ControllingPawn->GetActorRotation(),
		TargetRot,
		GetWorld()->DeltaTimeSeconds,
		TurnSpeed
	);

	// ȸ�� �� ����.
	ControllingPawn->SetActorRotation(CurrentRot);

	// ����.
	return EBTNodeResult::Succeeded;
}