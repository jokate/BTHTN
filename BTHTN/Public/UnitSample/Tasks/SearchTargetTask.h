// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNModule/HTNTask.h"
#include "SearchTargetTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BTHTN_API USearchTargetTask : public UHTNTask
{
	GENERATED_BODY()

public :
	virtual void TestActionLog() override;
};
