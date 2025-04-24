// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	// ������ ���̺� �ּ� �ε�.
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable"));
	if (DataTableRef.Object)
	{
		// UDataTable�� Key/Value�� ������.
		// Map���� ������� �ʰ�, �迭�� ���� ����.
		const UDataTable* DataTable = DataTableRef.Object;

		// ������ ���̺��� ����� �ʱ�ȭ�ƴ��� Ȯ��(����).
		check(DataTable->GetRowMap().Num() > 0);

		// Value ���� �����ϱ� ���� �迭.
		TArray<uint8*> ValueArray;

		// ������ ���̺��� GenerateValueArray �Լ��� ����� Value ���� �迭�� ����.
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		// �˰����� Ȱ���� Value ���� ��ȯ�� ����.
		//for (uint8* Value : ValueArray)
		//{
		//	CharacterStatTable.Add(*reinterpret_cast<FABCharacterStat*>(Value));
		//}

		Algo::Transform(ValueArray, CharacterStatTable, 
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
	}

	// �ִ� ���� �� ����.
	CharacterMaxLevel = CharacterStatTable.Num();

	// ���� ������ ������ Ȯ��.
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton& UABGameSingleton::Get()
{
	// �̱��� �������� (���� �ȵ�).
	// ������Ʈ ���ÿ� �̱������� �����߱� ������ ����.
	UABGameSingleton* Singleton = Cast<UABGameSingleton>(GEngine->GameSingleton);

	// ��ü�� ��ȿ�ϸ� ���۷����� ��ȯ.
	if (Singleton)
	{
		return *Singleton;
	}

	// �����ϸ� �ȵ�.
	// ���� ���.
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game singleton."));

	// �� ��ü ��ȯ.
	return *NewObject<UABGameSingleton>();
}