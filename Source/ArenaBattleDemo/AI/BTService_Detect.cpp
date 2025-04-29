// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AIController.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAI.h"

UBTService_Detect::UBTService_Detect()
{
	// �����Ϳ��� ���̴� �̸� ����.
	NodeName = TEXT("Detect");

	// Tick�� ����Ǵ� �ֱ� ����.
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// AIController�� �����ϴ� ��.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	// �����ϴ� ���� ������ �Լ� ����.
	if (!ControllingPawn)
	{
		return;
	}

	// �������̽��� ����ȯ.
	IABCharacterAIInterface* AIPawn 
		= Cast<IABCharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return;
	}

	// ���� ����.
	UWorld* World = ControllingPawn->GetWorld();

	// �浹 ���� ��� ���� ����.
	TArray<FOverlapResult> OverlapResults;

	// ���� ���� ��ġ.
	FVector Center = ControllingPawn->GetActorLocation();

	// ���� ���� ����.
	float DetectRadius = AIPawn->GetAIDetectRange();

	// �浹 ���� ��� ����.
	FCollisionQueryParams CollisionQueryParams(
		SCENE_QUERY_STAT(Detect),
		false,
		ControllingPawn
	);

	// ���� ó��.
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	// �浹�� �����Ǹ�,
	if (bResult)
	{
		// ������ �÷��̾ ���� ó�� ����.
		for (auto const& OverlapResult : OverlapResults)
		{
			// ������ ���͸� ������ ����ȯ.
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			// �÷��̾����� Ȯ��.
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// ������ �÷��̾ �����忡 ����.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(
					BBKEY_TARGET, Pawn
				);

				// ����� ��ο츦 Ȱ���� ���� ������ ������� ǥ��.
				DrawDebugSphere(
					World,
					Center,
					DetectRadius,
					16,
					FColor::Green,
					false,
					0.2f
				);

				// ������ ������ ������� ǥ��.
				DrawDebugPoint(
					World,
					Pawn->GetActorLocation(),
					10.0f,
					FColor::Green,
					false,
					0.2f
				);

				// ������ ������ ������� ǥ��.
				DrawDebugLine(
					World,
					ControllingPawn->GetActorLocation(),
					Pawn->GetActorLocation(),
					FColor::Green,
					false,
					0.2f
				);

				return;
			}
		}
	}

	// �÷��̾� ���� ����.
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	// ����� ��ο츦 Ȱ���� ���� ������ ���������� ǥ��.
	DrawDebugSphere(
		World,
		Center,
		DetectRadius,
		16,
		FColor::Red,
		false,
		0.2f
	);
}