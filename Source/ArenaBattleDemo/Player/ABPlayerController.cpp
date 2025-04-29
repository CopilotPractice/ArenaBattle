// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"

AABPlayerController::AABPlayerController()
{
	// ���� �������Ʈ �ּ¿��� Ŭ���� ���� �ε�.
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ������ �� ���콺�� ����Ʈ�� �ԷµǾ� �ٷ� ����ǵ��� ����.
	FInputModeGameOnly GameInput;
	SetInputMode(GameInput);

	// ���� ����.
	ABHUDWidget = CreateWidget<UABHUDWidget>(this, ABHUDWidgetClass);

	// ������ ���� ���� ���� ������,
	if (ABHUDWidget)
	{
		// ������ ȭ�鿡 �߰��� UI�� ���� �� �ֵ��� ����.
		ABHUDWidget->AddToViewport();
	}
}