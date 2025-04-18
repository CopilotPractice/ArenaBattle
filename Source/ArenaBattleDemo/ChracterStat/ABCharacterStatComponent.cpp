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
	//데미지 처리
	const float PrevHp = CurrentHp;

	// 데미지 보정 값
	// 오류로 인해 전달된 데미지가 음수일 수 있음
	// 음수인 경우는 0으로 설정
	// Clamp = Max(Min(X, MaxValue), MinValue) => Max와 Min을 넘기지 않음
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
	//현재 체력 업데이트
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	
	//체력 변경 이벤트 발행
	OnHpChanged.Broadcast(CurrentHp);
}

