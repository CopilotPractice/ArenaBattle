// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp) //null 타입 체크
	{
		//애니메이션을 소유한 액터가 인터페이스를 구현 했는지 확인
		IABAnimationAttackInterface* AttackInterface
			= Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());

		// 인터페이스를 구현한 경우 함수 호출
		if (AttackInterface)
		{
			AttackInterface->AttackHitCheck();
		}
	}

}
