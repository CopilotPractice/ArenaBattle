// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���÷��� ����� Ȱ��.

	// FABCharacterStat ����ü�� ������ �Ӽ��� �����ͼ� ����.
	for (
		TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());
		PropIt;
		++PropIt)
	{
		// �Ӽ��� Ű��(�̸� ��).
		const FName PropKey(PropIt->GetName());

		// Base ���ȿ� ���� �ؽ�Ʈ ��� �̸�.
		const FName TextBaseControlName
			= *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());

		// Modifier ���ȿ� ���� �ؽ�Ʈ ��� �̸�.
		const FName TextModifierControlName
			= *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		// �̸� ���� ����� �ؽ�Ʈ ��� �ε�.
		UTextBlock* BaseTextBlock
			= Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));

		// ����� ã������,
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		// �̸� ���� ����� �ؽ�Ʈ ��� �ε�.
		UTextBlock* ModifierTextBlock
			= Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));

		// ����� ã������,
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(
	const FABCharacterStat& BaseStat, 
	const FABCharacterStat& ModifierStat)
{
	// FABCharacterStat ����ü�� ������ �Ӽ��� �����ͼ� ����.
	for (
		TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct());
		PropIt;
		++PropIt)
	{
		// �Ӽ��� Ű��(�̸� ��).
		const FName PropKey(PropIt->GetName());

		// Base ���� ������ ������ ���� ����.
		float BaseData = 0.0f;

		// ���÷����� Ȱ���� �� ��������.
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);

		// Modifier ���� ������ ������ ���� ����.
		float ModifierData = 0.0f;

		// ���÷����� Ȱ���� �� ��������.
		//reinterpret_cast<const void*>(&ModifierStat);
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		// �ʿ��� Base �ؽ�Ʈ ����� ������ �� ����.
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(
				FText::FromString(FString::SanitizeFloat(BaseData))
			);
		}

		// �ʿ��� Modifier �ؽ�Ʈ ����� ������ �� ����.
		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(
				FText::FromString(FString::SanitizeFloat(ModifierData))
			);
		}
	}
}