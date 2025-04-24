// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	// ������ �� �޽� ������Ʈ ����.
	GetMesh()->SetHiddenInGame(true);
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