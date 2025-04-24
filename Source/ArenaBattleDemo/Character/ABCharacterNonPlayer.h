// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=ArenaBattle)
class ARENABATTLEDEMO_API AABCharacterNonPlayer : public AABCharacterBase
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
};