// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHPBarWidget::UABHPBarWidget(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer) 
{
	//�Ϻη� ��ȿ���� ���� ���� �����ϱ� ���� �� ����.
	MaxHp = -1.0f;
}

void UABHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	//MaxHp ���� ����� ���� �ƴ��� Ȯ��
	ensure(MaxHp > 0.0f); 

	// ���α׷����� ���� ������Ʈ
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);

	}
}

void UABHPBarWidget::NativeConstruct()//������ ������ �Ϸ� �ǰ� ȣ�� �� �� �ִ� �Լ�
{
	Super::NativeConstruct();

	//�� �Լ��� ȣ��Ǹ�, UI�� ���� ��κ��� ���� �ʱ�ȭ �Ǿ��ٰ� ������ �� ����

	//���� ���� ������ ���� �̸����� �˻�
	HpProgressBar = Cast<UProgressBar>( GetWidgetFromName(TEXT("PbHpBar"))); //������ ���α׷����� �̸��� ����
	ensure(HpProgressBar);

	// �ϰ� ���� �� : ĳ���Ϳ� �� ����(����)�� ����
	// �ٸ�, �������� �߻� = �������̽��� ���� ��ȸ�Ͼ� ����(������ ����)
	
	IABCharacterWidgetInterface* CharacterWidget= Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		//�������̽��� ���� ĳ���Ϳ� �� ����(����) ����
		CharacterWidget->SetupCharacterWidget(this);
	}
}
