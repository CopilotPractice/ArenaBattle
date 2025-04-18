// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "ABCharacterNonePlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEDEMO_API AABCharacterNonePlayer : public AABCharacterBase
{
	GENERATED_BODY()
	

public:
	AABCharacterNonePlayer();

protected:
	//죽음 상태 설정 함수
	virtual void SetDead();
};
