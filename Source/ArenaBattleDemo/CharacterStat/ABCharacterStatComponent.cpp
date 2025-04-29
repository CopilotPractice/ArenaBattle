// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	//MaxHp = 200.0f;
	//CurrentHp = MaxHp;
	CurrentLevel = 1.0f;
	AttackRadius = 50.0f;

	// bWantsInitializeComponent �÷��׸� �����ؾ�
	// InitializeComponent �Լ��� ȣ���.
	// ���� �� ������ �� �� �ֱ� ������ �ʿ��� ��쿡�� 
	// ����ϵ��� ������ ������ ����.
	bWantsInitializeComponent = true;
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ���� ���� ������ ����.
	SetLevelStat(static_cast<int32>(CurrentLevel));

	// ������ ���۵Ǹ� ü���� ���� ä���.
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// ��ȿ�� ���� ������ ����.
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);

	// ���� �̱������κ��� ���� ������ ��������.
	//BaseStat = UABGameSingleton::Get().GetCharacterStat(CurrentLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));

	// ������ ���� ���� ������ Ȯ��.
	check(BaseStat.MaxHp > 0.0f);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	// ����� ó��.
	const float PrevHp = CurrentHp;

	// ����� ���� ��.
	// ������ ���� ���޵� ������� ������ �� ����.
	// ������ ���� 0 ����.
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);

	// ����� ���.
	SetHp(PrevHp - ActualDamage);

	// �׾����� (ü���� ��� �����ߴ���) Ȯ��.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// �̺�Ʈ ����.
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	// ���� ü�� ������Ʈ.
	//CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	// ü�� ���� �̺�Ʈ ����.
	OnHpChanged.Broadcast(CurrentHp);
}