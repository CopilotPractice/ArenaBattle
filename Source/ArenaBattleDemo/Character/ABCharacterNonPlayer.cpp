// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// ������ �� �޽� ������Ʈ ����.
	GetMesh()->SetHiddenInGame(true);

	// AIController Ŭ���� ����.
	AIControllerClass = AABAIController::StaticClass();

	// ���� ��� ����.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// �޽� �ε� ��û.

	// NPCMeshes ��� ���� ����� �����ƴ��� Ȯ��.
	ensure(NPCMeshes.Num() > 0);

	//UE_LOG(LogTemp, Log, TEXT("NPCMeshes.Num(): %d"), NPCMeshes.Num());

	// �������� �ε��� ����.
	int32 RandomIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);

	// �񵿱�� �޽� �ε� ��û.
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		NPCMeshes[RandomIndex],
		FStreamableDelegate::CreateUObject(
			this, 
			&AABCharacterNonPlayer::NPCMeshLoadCompleted
		)
	);

}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// AI ��� ���� ����.
	//Controller
	AABAIController* ABAIController = Cast<AABAIController>(GetController());
	if (ABAIController)
	{
		ABAIController->StopAI();
	}

	// Ÿ�̸Ӹ� ����� ���� ����.
	FTimerHandle DeadTimerHandle;

	// Ÿ�̸� ����.
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				// ���� ����.
				Destroy();
			}
		),
		DeadEventDelayTime,		// Ÿ�̸� ���� �ð�.
		false					// �ݺ� ���� ����(�ݺ� ����).
	);
}

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	// �޽� ��û �� ��ȯ�� �ڵ� ���� ��ȿ���� Ȯ��.
	if (NPCMeshHandle.IsValid())
	{
		// �ε�� �ּ��� ���̷�Ż �޽� �ּ����� ��ȯ.
		USkeletalMesh* NPCMesh 
			= Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		
		// �޽� �ּ� ����.
		if (NPCMesh)
		{
			// ���̷�Ż �޽� �ּ� ����.
			GetMesh()->SetSkeletalMesh(NPCMesh);

			// ������Ʈ ���̵��� ����.
			GetMesh()->SetHiddenInGame(false);
		}
	}

	// �ּ� �ε忡 ����ߴ� �ڵ� ����.
	NPCMeshHandle->ReleaseHandle();
}

float AABCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AABCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AABCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AABCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AABCharacterNonPlayer::SetAIAttackDelegeate(
	const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AABCharacterNonPlayer::AttackByAI()
{
	// ���� ������ ���� �޺� ���� �Լ� ȣ��.
	ProcessComboCommand();
}

void AABCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();

	// ���� ���� ��������Ʈ ����.
	OnAttackFinished.ExecuteIfBound();
}