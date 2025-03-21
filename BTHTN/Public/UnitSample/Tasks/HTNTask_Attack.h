// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNModule/HTNTask.h"
#include "HTNTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class BTHTN_API UHTNTask_Attack : public UHTNTask
{
	GENERATED_BODY()

	virtual void TestActionLog() override;
};
