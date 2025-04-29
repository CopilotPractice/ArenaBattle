// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API UABCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	// ������ ������Ʈ�� �� ȣ���� �Լ�.
	void UpdateStat(
		const FABCharacterStat& BaseStat,
		const FABCharacterStat& ModifierStat
	);

private:
	// ���� ������ ���� UI ��Ʈ�� �߿� Base ���ȿ� �ش��ϴ� �ؽ�Ʈ ����� �����ϴ� ��.
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	// ���� ������ ���� UI ��Ʈ�� �߿� Modifier ���ȿ� �ش��ϴ� �ؽ�Ʈ ����� �����ϴ� ��.
	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
};