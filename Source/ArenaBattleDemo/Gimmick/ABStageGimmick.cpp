// Fill out your copyright notice in the Description page of Project Settings.

#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Item/ABItemBox.h"

// Sets default values
AABStageGimmick::AABStageGimmick()
{
	// State Section.
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	// ���ҽ� ����.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(
		this,
		&AABStageGimmick::OnStageTriggerBeginOverlap
	);

	// Gate Section.
	static FName GateSockets[] =
	{
		TEXT("+XGate"),
		TEXT("-XGate"),
		TEXT("+YGate"),
		TEXT("-YGate")
	};

	// ���ҽ� �ε�.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE"));

	// ����Ʈ ����.
	for (const FName& GateSocket : GateSockets)
	{
		// ������Ʈ ����.
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);

		// ������ ����ƽ �޽� ������Ʈ�� �ּ� ����.
		if (GateMeshRef.Object)
		{
			Gate->SetStaticMesh(GateMeshRef.Object);
		}

		// ������Ʈ ����.
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		// ������ ������Ʈ�� �ʿ� �߰�.
		Gates.Add(GateSocket, Gate);

		// ������ ��ġ�� BoxComponent ���� �� ����.
		FName TriggerName = *GateSocket.ToString().Append("Trigger");

		// ������Ʈ ����.
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);

		// ������Ʈ ����.
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(
			this,
			&AABStageGimmick::OnGateTriggerBeginOverlap
		);

		// �±� ����.
		GateTrigger->ComponentTags.Add(GateSocket);

		// ������ ������Ʈ�� �迭�� �߰�.
		GateTriggers.Add(GateTrigger);
	}

	// ������ ���� �غ� ���·� ����.
	CurrentState = EStageState::Ready;

	// ������ - ��������Ʈ �� ����.
	StageChangedActions.Add(
		EStageState::Ready,
		FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady)
	);

	StageChangedActions.Add(
		EStageState::Fight,
		FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight)
	);

	StageChangedActions.Add(EStageState::Reward, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward));

	StageChangedActions.Add(EStageState::Next, FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext));

	// Fight Section.
	OpponentSpawnTime = 2.0f;

	// ������ NPC Ŭ���� Ÿ�� ����.
	OpponentClass = AABCharacterNonPlayer::StaticClass();

	// Reward Section.

	// ������ ������ ������ Ŭ���� Ÿ�� ����.
	RewardItemClass = AABItemBox::StaticClass();

	// ���� ��ġ ����.
	for (const FName& GateSocket : GateSockets)
	{
		// ���� ��ġ�� ����� ��ġ �� ���ϱ�.
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;

		// �ʿ� �߰�.
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// OnConstruction�� ȣ��� �� ���� ���� ���ŵǵ��� ó��.
	SetState(CurrentState);
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	// ���� ���� ������Ʈ.
	CurrentState = InNewState;

	// ���޵� ���¿� �ʿ� ���Ե� ������ ��������Ʈ ����.
	if (StageChangedActions.Contains(InNewState))
	{
		StageChangedActions[CurrentState].StageChangedDelegate.ExecuteIfBound();
	}
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ĳ���Ͱ� ���������� �����ϸ� ���� ���·� ��ȯ.
	SetState(EStageState::Fight);
}

void AABStageGimmick::SetReady()
{
	// ��� Ʈ���� Ȱ��ȭ.
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// �÷��̾ ����Ʈ�� ��ȣ�ۿ����� �ʵ��� �ݸ��� ����.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// �غ� ������ ���� ���� �����ֵ��� ����.
	OpenAllGates();
}

void AABStageGimmick::SetFight()
{
	// ��� Ʈ���� Ȱ��ȭ.
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// �÷��̾ ����Ʈ�� ��ȣ�ۿ����� �ʵ��� �ݸ��� ����.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// ��� �� �ݱ�.
	CloseAllGates();

	// NPC ����.
	GetWorld()->GetTimerManager().SetTimer(
		OpponentTimerHandle,				// Ÿ�̸� �ڵ�.
		this,								// �ݹ� �Լ� ���� ��ü.
		&AABStageGimmick::OpponentSpawn,	// �ݹ� �Լ�.
		OpponentSpawnTime,					// Ÿ�̸� �ð� ��.
		false								// �ݺ� ����.
	);
}

void AABStageGimmick::SetChooseReward()
{
	// ��� Ʈ���� Ȱ��ȭ.
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// �÷��̾ ����Ʈ�� ��ȣ�ۿ����� �ʵ��� �ݸ��� ����.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// ��� �� �ݱ�.
	CloseAllGates();

	// ���� ���� ����.
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	// ��� Ʈ���� Ȱ��ȭ.
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// �÷��̾ ����Ʈ�� ��ȣ�ۿ����� �ʵ��� �ݸ��� ����.
	for (const auto& GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}

	// ��� �� ���� (�ٸ� ���������� �̵��� �� �ֵ���).
	OpenAllGates();
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ����Ʈ���� �ϳ��� �±׸� �����߱� ������ �̸� Ȯ��.
	ensure(OverlappedComponent->ComponentTags.Num() == 1);

	// �±� Ȯ�� (��: +XGate)
	FName ComponentTag = OverlappedComponent->ComponentTags[0];

	// �±׿��� ���������� ��ġ�� ������ �̸��� ��������.
	FName SocketName = FName(*ComponentTag.ToString().Left(2));

	// ������ �ִ��� Ȯ��.
	check(Stage->DoesSocketExist(SocketName));

	// ���� �̸��� ���� ��ġ �� ��������.
	FVector NewLocation = Stage->GetSocketLocation(SocketName);

	// ������ ��ġ�� �̹� �ٸ� ���������� ������ Ȯ��.
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParams(
		SCENE_QUERY_STAT(GateTrigger),
		false,
		this
	);

	// ���������� �˻�.
	bool Result = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,		// �浹 ����� ��ȯ�� ����.
		NewLocation,		// �浹 ������ ��ġ.
		FQuat::Identity,	// ȸ��.
		// �浹 ��� ������Ʈ ä��.
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		// �浹 ������ �� ����� ����.
		FCollisionShape::MakeSphere(775.0f),
		// �ݸ��� �ɼ�(�ڱ�� �����ϱ� ����).
		CollisionQueryParams
	);

	// �����Ϸ��� ��ġ�� �ٸ� ���������� ���ٸ� ���� ����.
	if (!Result)
	{
		FTransform SpawnTransform(NewLocation);

		AABStageGimmick* NewGimmick
			= GetWorld()->SpawnActorDeferred<AABStageGimmick>(
				AABStageGimmick::StaticClass(),
				SpawnTransform
			);

		// ���� ������ ���������� ���ڸ� �ϳ� ����.
		if (NewGimmick)
		{
			NewGimmick->SetStageNum(CurrentStageNum + 1);

			// ���� �Ϸ� ó��.
			NewGimmick->FinishSpawning(SpawnTransform);
		}
	}
}

void AABStageGimmick::OpenAllGates()
{
	// �� ������ �迭�� ��ȸ�ϸ鼭 ȸ�� ����.
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	// �� ������ �迭�� ��ȸ�ϸ鼭 ȸ�� ����.
	for (const auto& Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OpponentDestroyed(AActor* DestroyedActor)
{
	// NPC�� ������ ���� �ܰ�� ����.
	SetState(EStageState::Reward);
}

void AABStageGimmick::OpponentSpawn()
{
	// NPC�� ������ ��ġ ����.
	const FTransform SpawnTransform(
		GetActorLocation() + FVector::UpVector * 88.0f
	);

	// NPC ����.
	AABCharacterNonPlayer* ABOpponentCharacter
		= GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(
			OpponentClass, SpawnTransform
		);

	// NPC�� �׾��� �� ����Ǵ� ��������Ʈ�� ���.
	//AABCharacterNonPlayer* ABOpponentCharacter = Cast<AABCharacterNonPlayer>(OpponentActor);
	if (ABOpponentCharacter)
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OpponentDestroyed);

		// ���� �������� ������ ĳ���� NPC ������ ����.
		ABOpponentCharacter->SetLevel(CurrentStageNum);

		// ���� �Ϸ� ó��.
		ABOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

void AABStageGimmick::OnRewardTriggerBeginOvelap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// ĳ���Ͱ� ���� ���ڸ� ȹ���ϸ�, ���� �迭�� ��ȸ�ϸ鼭 ó�� ����.
	for (const auto& RewardBox : RewardBoxes)
	{
		// RewardBoxes�� �׸��� �������� �ϱ� ������ �ش� �����Ͱ� ��ȿ���� ������ �� ����.
		// ���� ���ڰ� ��ȿ�ϸ� ó�� ����.
		if (RewardBox.IsValid())
		{
			// ���� ������ ������ ��������.
			AABItemBox* ValidBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();

			// �� �ڽ��� ���� �ٸ� ��쿡�� ����.
			if (OverlappedBox != ValidBox)
			{
				ValidBox->Destroy();
			}
		}
	}

	// ���� �ܰ�� ��ȯ.
	SetState(EStageState::Next);
}

void AABStageGimmick::SpawnRewardBoxes()
{
	// �ڽ� ���� ��ġ�� ���� ��ȸ�ϸ鼭 ���� ó��.
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		// �ڽ� ���� ��ġ.
		FVector SpawnLocation
			= GetActorLocation()
			+ RewardBoxLocation.Value
			+ FVector(0.0f, 0.0f, 30.0f);

		FTransform SpawnTransform(SpawnLocation);

		// �ڽ� ���� ����.
		// UObject / Actor.
		AABItemBox* RewardBoxActor
			= GetWorld()->SpawnActorDeferred<AABItemBox>(
				RewardItemClass,
				SpawnTransform
			);

		// ������ �� ������, ������ �ڽ� Ÿ������ ����ȯ.
		//AABItemBox* RewardBoxActor = Cast<AABItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			// ������ ������ ���Ϳ� �±� �߰�.
			// ���߿� ������ �ϱ� ���� �߰�.
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);

			// ������ �̺�Ʈ�� ���.
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(
				this, &AABStageGimmick::OnRewardTriggerBeginOvelap
			);

			// ������ ������ ���ڸ� �迭�� �߰�.
			RewardBoxes.Add(RewardBoxActor);
		}
	}

	// ������ ��� �Ϸ��� �Ŀ� FinishSpawning() ȣ��.
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox.Get()->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
	}
}