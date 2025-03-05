// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNModule/TaskWorldState.h"
#include "AttackerWorldState.generated.h"

/**
 * 
 */
UCLASS()
class BTHTN_API UAttackerWorldState : public UTaskWorldState
{
	GENERATED_BODY()

public :
	UAttackerWorldState();

	virtual void UpdateWorldBooleanValue(FName KeyName, bool UpdatedValue) override;

	virtual void UpdateWorldFloatValue(FName KeyName, float UpdatedValue) override;

	virtual void UpdateWorldIntegerValue(FName KeyName, int32 UpdatedValue) override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Attacker World")
	FTaskRelatedValue_Boolean TargetFound;

	UPROPERTY(BlueprintReadWrite, Category = "Attacker World")
	FTaskRelatedValue_Int CurrentHp;
};
