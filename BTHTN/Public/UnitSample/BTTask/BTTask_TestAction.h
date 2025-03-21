﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TestAction.generated.h"

/**
 * 
 */
UCLASS()
class BTHTN_API UBTTask_TestAction : public UBTTaskNode
{
	GENERATED_BODY()

public :
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
