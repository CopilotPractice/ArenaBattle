// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree//BlackboardComponent.h"
#include "ABAI.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{

    NodeName = TEXT("CanAttack");


}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
  
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    // AIController가 제어하는 폰
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

    //폰이 없으면 종료
    if (!ControllingPawn)
    {
        return false;
    }

    //인터페이스로 형변환  => 다형성
    IABCharacterAIInterface* AIPawn = Cast< IABCharacterAIInterface>(ControllingPawn);
    if (!AIPawn) // null이면 형변환(구현) 안 되있음
    {
        return false;
    }

    // 블랙 보드에 저장된 캐릭터 값
    //캐릭터 데이터 타입으로 받을 수 있는데, 
    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));

    //블랙보드에 저장된 값이 없으면 종료
    if (!Target)
    {
        return false;
    }

    // 캐릭터와의 거리 계산
    float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);


    // 공격 가능 범위 값
    float AttackRangeWithRadius = AIPawn->GetAIAttackRange(); 

    //캐릭터와의 거리가 공격 가능거리보다 가까운지 확인 후 결과 반환
    bResult = (DistanceToTarget <= AttackRangeWithRadius);
    return bResult;

}
