// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "HTNBTPlanner.generated.h"

class UHTNBTComponent;
class UHTNTask;

// This class does not need to be modified.
UINTERFACE()
class UHTNBTPlanner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BTHTN_API IHTNBTPlanner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UHTNBTComponent* GetHTNBTComponent() { return nullptr; }
	virtual FGameplayTag GetTaskTagToActive() { return FGameplayTag(); }

	virtual void OnTaskFinished( FGameplayTag& FinishedTaskTag ) {};

	virtual void ResetAllTaskTag() {};

	virtual UHTNTask* GetFirstTaskInPlan() { return nullptr; }

	virtual void UpdateWorldIntegerValue( FName KeyName, int32 UpdatedValue ) {};
	virtual void UpdateWorldBooleanValue( FName KeyName, bool UpdatedValue ) {};
	virtual void UpdateWorldFloatValue( FName KeyName, float UpdatedValue ) {};
};
