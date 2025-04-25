// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABAIController : public AAIController
{
	GENERATED_BODY()

public:
	AABAIController();
	
	//AI 실행하는 함수
	void RunAI();

	//중지
	void StopAI();

protected:

	// 컨트롤러가 폰에 빙의했을 때 실행되는 함수
	virtual void OnPossess(APawn* InPawn) override;

	//Data Section
private:
	// Blackboard 애셋
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	//BT 에셋
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	//호출 시점
	//  맵생성 -> 월드 생성 -> 레벨 생성 -> 엑터 생성 -> (게임 모드 확인)
};
