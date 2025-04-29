// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AIController.h"
#include "Physics/ABCollision.h"
#include "Interface//ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAI.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	
	//Tick이 실행되는 주기 설정
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//AIController가 제어하는 폰
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!ControllingPawn)
	{
		return;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (!AIPawn)
	{
		return;
	}

	//월드 정보
	UWorld* World = ControllingPawn->GetWorld();

	//충돌 감지 결과 처리 , 나중을 위해서 배열로 저장
	TArray<FOverlapResult> OverlapResults;


	FVector Center = ControllingPawn->GetActorLocation();

	float DetectRadius = AIPawn->GetAIDetectRange();

	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	// 플레이어 감지 처리
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
		);

	//충돌 감지되면
	if (bResult)
	{

		// 감지 플레이어 대해 처리 (변환하지말라고 const붙힘)
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			//플레이어인지 확인
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// 감지한 플레이어를 블랙보드에 저장
				// Pawn이 Object 타입이기 때문에 SetValueAsObject()를 사용
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(
					BBKEY_TARGET, Pawn
				);

				//디버그 드로우를 활용하여 감지 영역을 녹색으로 표시
				DrawDebugSphere
					(World,
					Center, 
					DetectRadius, 
					16, 
					FColor::Green, 
					false,
					0.2f);

				DrawDebugPoint(
					World,
					Pawn->GetActorLocation(),
					10.0f,
					FColor::Green,
					false,
					0.2f
				);

				//감지된 방향을 녹색으로 표시
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

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	//디버그 드로우를 활용하여 감지 영역을 빨간색 표시
	DrawDebugSphere
		(
			World,
			Center,
			DetectRadius,
			16,
			FColor::Red,
			false,
			0.2f
		);
}
