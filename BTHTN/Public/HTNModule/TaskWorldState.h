// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNTaskConditionalValue.h"
#include "UObject/Object.h"
#include "TaskWorldState.generated.h"

/**
 * 
 */
UCLASS()
class BTHTN_API UTaskWorldState : public UObject
{
	GENERATED_BODY()

	//내부에 있는 멤버들에 대한 정보를 저장해야 합니다.
	void SetupStructProperties();
	
	UPROPERTY(VisibleAnywhere)
	FTaskWorldStateData TaskWorldState;
};
