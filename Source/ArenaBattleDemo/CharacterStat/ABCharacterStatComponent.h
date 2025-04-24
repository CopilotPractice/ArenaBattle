// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

// ��������Ʈ ����.
// ü�� ���� 0�� �Ǿ��� �� ������ ��������Ʈ.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

// ü�� ������ �߻��� �� ������ ��������Ʈ.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLEDEMO_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Getter.
	// __forceinline.
	//FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }

	// ĳ���� ������ �����ϴ� �Լ�.
	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	// �ΰ� ���ȵ����� ���� �Լ�.
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
	}

	// ��ü ���� ������ ��ȯ �Լ�.
	FORCEINLINE FABCharacterStat GetTotalStat() const
	{ 
		return BaseStat + ModifierStat;
	}

	// ����� ���� �Լ�.
	float ApplyDamage(float InDamage);

protected:
	// HP�� ������� �� ������ �Լ�.
	void SetHp(float NewHp);

public:
	// ü���� ��� �������� �� ����Ǵ� ��������Ʈ.
	FOnHpZeroDelegate OnHpZero;

	// ü�� ���� ��������Ʈ.
	FOnHpChangedDelegate OnHpChanged;

	// ����.
protected:

	// ������ �ӽ÷� ����ϴ� ������ ����(��Ȱ��ȭ).
	//// �ִ� ü�� ��.
	//UPROPERTY(VisibleInstanceOnly, Category = Stat)
	//float MaxHp;

	// ���� ü�� ��.
	// Transient: ���� ü�� ���� ������ ������ ������ �ٲ�� ��.
	// ���� ��ũ�� ��������� ������ �ʿ����� ���� �� ����.
	// �̷� ���� Transient�� ���� ����.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// ���� ����.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	// ĳ������ �⺻ ���� ������.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	// �ΰ� ���� ������.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;
};