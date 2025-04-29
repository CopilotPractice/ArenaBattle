// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABAI.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI Controller가 제어하는 폰
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));

	if (!TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	//인터페이스 형변환
	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	float TurnSpeed = AIPawn->GetAITurnSpeed();

	//바라볼 방향 구하기
	FVector LookVector = TargetPawn->GetTargetLocation() - ControllingPawn->GetActorLocation();

	//높이 성분은 제거
	LookVector.Z = 0.0f;

	//바라볼 방향을 이용하여 FRotator 구하기
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	//부드럽게 회전 설정
	
	//현재 프레임에 설정할 회전 값
	FRotator CurrentRot = FMath::RInterpTo(
		ControllingPawn->GetActorRotation(),
		TargetRot,
		GetWorld()->DeltaTimeSeconds,
		TurnSpeed
	); // Rotate 

	ControllingPawn->SetActorRotation(CurrentRot);

	return EBTNodeResult::Type();
}
