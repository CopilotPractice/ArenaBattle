// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=ArenaBattle)
class ARENABATTLEDEMO_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
{
	GENERATED_BODY()

public:
	AABCharacterNonPlayer();

protected:

	// �������� �޽� �ε� ��û�� ���� �Լ�.
	virtual void PostInitializeComponents() override;

	// ���� ���� ���� �Լ�.
	virtual void SetDead() override;

	// NPC �޽� �ε尡 �Ϸ�� �� ȣ��� �ݹ�.
	void NPCMeshLoadCompleted();

	// config ���Ͽ� ������ ��� ���� �迭�� ����.
	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	// �迭�� ä���� ��θ� Ȱ���� �񵿱�� �ּ��� �ε��� �� ���.
	TSharedPtr<FStreamableHandle> NPCMeshHandle;

protected:
	//NPC ĳ���Ͱ� �ʼ������� �����ؾ��� �Լ�
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	// ĳ���Ϳ��� ��������Ʈ�� �ѱ� �� ����� �Լ�
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinised) override;

	//AI ������ �� ����� �Լ�
	virtual void AttackByAI() override;

	// SetAIAttackDelegate �Լ��� ���޵� ��������Ʈ�� ������ �Լ�
	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};