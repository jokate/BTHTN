// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HTNTaskConditionalValue.h"
#include "UObject/Object.h"
#include "HTNTask.generated.h"

/**
 * 
 */
DEFINE_LOG_CATEGORY_STATIC( HTNTaskLog, Log, All);
// 2025-03-21 Need To Change                                             
UCLASS(Blueprintable, BlueprintType)
class BTHTN_API UHTNTask : public UObject
{
	GENERATED_BODY()

public :
	//Precondition Checker
	//Notice : Precondition should return true to activate HTN Task.
	virtual bool CheckPrecondition();

	//Need To Register HTN Component To Active
	//This Task will be managed by AI Owner
	virtual void InitializeHTNComponent( class UHTNBTComponent* BTComponent );

	//This function will calculate effect.
	//this effect will be marked in temp world state.
	virtual void SimulateEffectToOwner( bool bIsAdded );
	

	virtual bool IsTagForTask( FGameplayTag& InGameplayTag ) const { return TaskTag == InGameplayTag; }
	
#pragma region Getter Function

	virtual AActor* GetOwner() const;

	virtual UHTNBTComponent* GetHTNBTComponent() const;
	
	virtual FGameplayTag GetTaskTag() const { return TaskTag; }

	virtual FGameplayTagContainer& GetPossibleNextTag() { return PossibleNextTags; }

	virtual EHTNTaskType GetTaskType() const { return HTNTaskType; }

	virtual FGameplayTagContainer& GetSequenceTagContainer() { return SequenceTags; }

#pragma endregion 

#if UE_EDITOR
	virtual void TestActionLog() {};
#endif

protected :

	void ApplyTaskSimulateValue( FTaskSimulateValue& SimulateValue, bool IsAdded );
	
protected :
	
	//On Task Registered Weak Pointer of Owner Will be Allocated.
	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;
	
	UPROPERTY()
	TWeakObjectPtr<class UHTNBTComponent> OwnerBTComponent;

	//GameplayTag for Task : it will be used for tree search to find possible next tag during planning search
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TaskTag;

	// Possible Task Tag : if planning task finished, planner will find next task by this tags.
	// 
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer PossibleNextTags;

	// Next Sequence Tag ( Compound Task Only ) 
	UPROPERTY( EditDefaultsOnly )
	FGameplayTagContainer SequenceTags;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FTaskSimulateValue_Compare> SimulationValues;

	//Affect World State.
	UPROPERTY(EditDefaultsOnly)
	TArray<FTaskSimulateValue> ApplySimulateValues;

	UPROPERTY( EditDefaultsOnly )
	EHTNTaskType HTNTaskType;
	//Todo. Cost function need to declared because of flexibility.
};