// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "ABCharacterBase.generated.h"

// �α� ī�װ� �߰�.
DECLARE_LOG_CATEGORY_EXTERN(LogABCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter
};

// ������ ȹ�� ó���� ���� ��������Ʈ ����.
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/)

// ��������Ʈ�� �ټ��� �迭(��)�� �����ϱ� ���� ����ü ����.
// ��������Ʈ ��ü�� ���ڷ� ����� �� ���� ������ ���� ����ü ������ �ʿ���.
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()

	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate)
		: ItemDelegate(InItemDelegate) {
	}

	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class ARENABATTLEDEMO_API AABCharacterBase
	: public ACharacter,
	public IABAnimationAttackInterface,
	public IABCharacterWidgetInterface,
	public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

	virtual void SetCharacterControlData(const class UABCharacterControlData* InCharacterControlData);

	virtual void SetupCharacterWidget(class UUserWidget* InUserWidget) override;

	// ���� ���� �Լ� (�ִ� ��Ƽ���̷κ��� ȣ���).
	virtual void AttackHitCheck() override;

	// ����� ó�� �Լ�.
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	// ������Ʈ�� �ʱ�ȭ�� ���Ŀ� ȣ��Ǵ� �Լ�.
	virtual void PostInitializeComponents() override;

	// Combo Section.
protected:

	// �޺� �׼� ó�� �Լ�.
	// ������ ó�� ����� ���� �޺� �׼� ó���� �б�.
	void ProcessComboCommand();

	// �޺� �׼��� ���۵� �� ȣ���� �Լ�.
	void ComboActionBegin();

	// �޺��� ����� �� ȣ��� �Լ�.
	// �ִ� ��Ÿ�ֿ��� �����ϴ� ��������Ʈ�� �Ķ���� ����.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	//NPC�� ������ ������ ������ �� �� �ֵ��� �Լ� �߰�
	virtual void NotifyComboActionEnd(); // 

	// �޺� Ÿ�̸� ���� �Լ�.
	void SetComboCheckTimer();

	// Ÿ�̸� �ð� ���̿� �Է��� ���Դ��� ���θ� Ȯ���ϴ� �Լ�.
	void ComboCheck();

	// Dead Section.
protected:

	// ���� ���� ���� �Լ�.
	virtual void SetDead();

	// �״� �ִϸ��̼� ��� �Լ�.
	void PlayDeadAnimation();

protected:
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

	// ���� ��Ÿ�� �ּ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// �޺� ó���� ����� ������ �ּ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	// ���� ��� ���� �޺� �ܰ�.
	// 0-> �޺� �������� ����. 1/2/3/4 �޺��� ���۵�.
	int32 CurrentCombo = 0;

	// �޺� ���� ���θ� �Ǵ��ϱ� ���� Ÿ�̸� �ڵ�.
	FTimerHandle ComboTimerHandle;

	// �޺� Ÿ�̸� ������ �Է��� ���Դ����� Ȯ���ϴ� �Ҹ��� ����.
	bool HasNextComboCommand = false;

	// Dead Section.
protected:

	// ���� ��Ÿ�� �ּ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	// ���� �ڿ� ���͸� �����ϱ� ������ ����� �ð� ��.
	float DeadEventDelayTime = 5.0f;

	// Stat/Widget Section.
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;

	// Item Section.
protected:

	// ���� ����ü�� ������ �� �ִ� �迭.
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	// ������ ȹ�� �� ȣ��� �Լ�.
	virtual void TakeItem(class UABItemData* InItemData) override;

	// ������ �������� ó���� �Լ� ����.
	virtual void DrinkPortion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

	// ���� �������� ȹ������ �� ����� ���̷�Ż �޽� ������Ʈ.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	// Stat Section.
public:

	// ���� Getter/Setter.
	int32 GetLevel() const;
	void SetLevel(int32 InNewLevel);
};