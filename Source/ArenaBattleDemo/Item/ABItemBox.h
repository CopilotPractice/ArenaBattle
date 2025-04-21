// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLEDEMO_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// 박스 컴포넌트의 오버랩 델리게이트에 등록 할 함수
	//OnComponentBeginOverlap 델리기ㅔ이트는 다이나믹으로 지정되어 있기 떄문에
	//UFUCTION 매크로로 지정해야함

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 파티클 재생 종료 시 발행되는 델리게이트에 등록할 함수
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);


protected:
		// 액터의 충돌을 담당할 박스 컴포넌트
		UPROPERTY(VisibleAnywhere, category=Box)
		TObjectPtr<class UBoxComponent> Trigger;

		// 아이템 박스를 보여줄 메시 컴포넌트
		UPROPERTY(VisibleAnywhere, category = Box)
		TObjectPtr<class UStaticMeshComponent> Mesh;

		UPROPERTY(VisibleAnywhere, category = Box)
		TObjectPtr<class UParticleSystemComponent> Effect;
		// 전방선언으로 컴파일 시간 낮춤

		//아이템 정보
		UPROPERTY(EditAnywhere, Category = Item)
		TObjectPtr<class UABItemData> Item;
};
