// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "HTNTask.generated.h"

/**
 * 
 */


UCLASS(Blueprintable)
class BTHTN_API UHTNTask : public UObject
{
	GENERATED_BODY()

public :
	//Precondition Checker
	virtual bool CheckPrecondition() { return true; }

	//Need To Register HTN Component To Active
	//This Task will be managed by AI Owner
	virtual void InitializeHTNComponent( class UHTNBTComponent* BTComponent );

	//Do Action To Owner
	virtual void Action() {};

	//Effect To Owner After Action. (Real Action)
	virtual void EffectToOwner() {}

	//This function will calculate effect.
	//this effect will marked in temp world state.
	virtual void SimulateEffectToOwner() {}

	virtual bool IsTagForTask( FGameplayTag& InGameplayTag ) const { return TaskTag == InGameplayTag; }

private :
	//On Task Registered Weak Pointer of Owner Will be Allocated.
	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	//GameplayTag for Task : it will be used for tree search to find possible next tag during planning search
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TaskTag;

	// Possible Task Tag : if planning task finished, planner will find next task by this tags.
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PossibleNextTags;

	//Todo. Cost function need to declared because of flexibility.
};
