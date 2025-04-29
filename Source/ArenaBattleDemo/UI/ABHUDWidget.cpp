// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"
#include "ABHpBarWidget.h"
#include "ABCharacterStatWidget.h"
#include "Interface/ABCharacterHUDInterface.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UABHUDWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	// HpBar�� �ִ� ü�� ���� ����.
	FABCharacterStat TotalStat = (BaseStat + ModifierStat);
	HpBar->SetMaxHp(TotalStat.MaxHp);

	// ĳ���� ���ȿ� ���ο� ������ ����.
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �̸����� �˻��� HpBar ���� ����.
	HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	// ����� �ε� �ƴ��� Ȯ��.
	ensure(HpBar != nullptr);

	// �̸����� CharacterStat ���� ����.
	CharacterStat = Cast<UABCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	// ����� �ε� �ƴ��� Ȯ��.
	ensure(CharacterStat != nullptr);

	//GetOwningPlayer()->GetPawn();
	IABCharacterHUDInterface* HUDPawn 
		= Cast<IABCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}