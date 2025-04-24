// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "ABItemData.h"

// Sets default values
AABItemBox::AABItemBox()
{
	// ������Ʈ ����.
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	// ������Ʈ�� ���� ����.
	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	// �ݸ��� �������� ����.
	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	// �ڽ��� ũ�� ����.
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	//// Ʈ���Ű� �߻��ϴ� ���̳��� ��������Ʈ�� �Լ� ���.
	//Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	// �ּ� �ε�.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	// �޽� ������Ʈ�� ��ġ ����.
	Mesh->AddRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// ��ƼŬ �ּ� �ε�.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (EffectRef.Object)
	{
		// ��ƼŬ �ּ� ����.
		Effect->SetTemplate(EffectRef.Object);

		// �ٷ� ������� �ʵ��� ����.
		Effect->bAutoActivate = false;
	}

	//Trigger->OnComponentBeginOverlap
	//Effect->OnSystemFinished
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// PrimaryAssetId ����� �������� ���� �ּ� �Ŵ��� �̱��� ����.
	UAssetManager& Manager = UAssetManager::Get();

	// �ּ� ��� ��������.
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);

	// ����� �����Դ��� �˻�.
	ensure(Assets.Num() > 0);

	//UE_LOG(LogTemp, Log, TEXT("Assets.Num(): %d"), Assets.Num());

	// �������� �ε��� ����.
	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);

	// ���õ� �ε����� ����� �ּ� ����.
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));

	// �ּ��� �ε尡 �ȵ� ���, �ε�.
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	// �ε��� �ּ��� ���������� ����.
	Item = Cast<UABItemData>(AssetPtr.Get());

	// ����� �����ƴ��� Ȯ��.
	ensure(Item);

	// Ʈ���Ű� �߻��ϴ� ���̳��� ��������Ʈ�� �Լ� ���.
	Trigger->OnComponentBeginOverlap.AddDynamic(
		this, &AABItemBox::OnOverlapBegin
	);
}

void AABItemBox::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// �� ���ڵ� �ִٰ� ����.
	// Item�� nullptr�̸� ��.
	if (!Item)
	{
		Destroy();
		return;
	}

	// �������� ������ ĳ���Ϳ� ������ ȹ�� �޽��� ����.
	IABCharacterItemInterface* OvelappedPawn 
		= Cast<IABCharacterItemInterface>(OtherActor);
	if (OvelappedPawn)
	{
		OvelappedPawn->TakeItem(Item);
	}

	// ��ƼŬ ���.
	Effect->Activate();

	// �޽ô� �Ⱥ��̵��� ó�� (��Ȱ��ȭ).
	Mesh->SetHiddenInGame(true);

	// ������ �ݸ��� ����.
	SetActorEnableCollision(false);

	// ��ƼŬ ��� ���� �� ����Ǵ� ��������Ʈ�� �Լ� ���.
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	// ��ƼŬ ����� �Ϸ�Ǹ�, ���� ����.
	Destroy();
}