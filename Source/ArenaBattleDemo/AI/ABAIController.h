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
	
	//AI �����ϴ� �Լ�
	void RunAI();

	//����
	void StopAI();

protected:

	// ��Ʈ�ѷ��� ���� �������� �� ����Ǵ� �Լ�
	virtual void OnPossess(APawn* InPawn) override;

	//Data Section
private:
	// Blackboard �ּ�
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	//BT ����
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	//ȣ�� ����
	//  �ʻ��� -> ���� ���� -> ���� ���� -> ���� ���� -> (���� ��� Ȯ��)
};
