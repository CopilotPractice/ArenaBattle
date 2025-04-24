#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ABCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterStat()
		: MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f)
	{
	}

	// ������ �����ε�.
	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		// ���� ������ ������.
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);

		// ������ ������ ������.
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		// ��� ������ ������.
		FABCharacterStat Result;

		// ��� ����ü ������.
		float* ResultPtr = reinterpret_cast<float*>(&Result);

		// ����ü�� ���Ե� float ������ ���� Ȯ��.
		int32 StatNum = sizeof(FABCharacterStat) / sizeof(float);

		// �����͸� Ȱ���� ���� ó��.
		for (int32 ix = 0; ix < StatNum; ++ix)
		{
			ResultPtr[ix] = ThisPtr[ix] + OtherPtr[ix];
		}
		
		// ��� ��ȯ.
		return Result;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
};