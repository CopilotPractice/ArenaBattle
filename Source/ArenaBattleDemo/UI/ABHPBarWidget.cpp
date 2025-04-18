// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHPBarWidget::UABHPBarWidget(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer) 
{
	//일부러 유효하지 않은 값을 구분하기 위해 값 설정.
	MaxHp = -1.0f;
}

void UABHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	//MaxHp 값이 제대로 설정 됐는지 확인
	ensure(MaxHp > 0.0f); 

	// 프로그레스바 위젯 업데이트
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);

	}
}

void UABHPBarWidget::NativeConstruct()//위젯의 설정이 완료 되고 호출 할 수 있는 함수
{
	Super::NativeConstruct();

	//이 함수가 호출되면, UI에 대한 대부분의 값이 초기화 되었다고 생각할 수 있음

	//위젯 참조 설정을 위해 이름으로 검색
	HpProgressBar = Cast<UProgressBar>( GetWidgetFromName(TEXT("PbHpBar"))); //위젯의 프로그래스바 이름을 지정
	ensure(HpProgressBar);

	// 하고 싶은 것 : 캐릭터에 내 정보(위젯)를 전달
	// 다만, 강참조가 발생 = 인터페이스를 통해 우회하야 전달(느슨한 결합)
	
	IABCharacterWidgetInterface* CharacterWidget= Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		//인터페이슬를 통해 캐릭터에 내 정보(위젯) 전달
		CharacterWidget->SetupCharacterWidget(this);
	}
}
