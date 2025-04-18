// Fill out your copyright notice in the Description page of Project Settings.


#include "ChracterStat/ABCharacterStatComponent.h"
//#include "ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
	: MaxHp(200.0f), CurrentHp(MaxHp)
{

	//MaxHp = 200.0f;
	//CurrentHp = MaxHp;

}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
	
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	//������ ó��
	const float PrevHp = CurrentHp;

	// ������ ���� ��
	// ������ ���� ���޵� �������� ������ �� ����
	// ������ ���� 0���� ����
	// Clamp = Max(Min(X, MaxValue), MinValue) => Max�� Min�� �ѱ��� ����
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);


	SetHp(PrevHp - ActualDamage);

	
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	//���� ü�� ������Ʈ
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	
	//ü�� ���� �̺�Ʈ ����
	OnHpChanged.Broadcast(CurrentHp);
}

