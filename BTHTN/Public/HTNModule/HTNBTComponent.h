﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HTNTask.h"
#include "HTNTaskConditionalValue.h"
#include "Components/ActorComponent.h"
#include "Interface/HTNBTPlanner.h"
#include "HTNBTComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BTHTN_API UHTNBTComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHTNBTComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void RegisterTask( TSubclassOf<UHTNTask> HTNTaskClass );

#pragma region Task Planning
	// Plan Task.
	virtual void SimulatePlanningTask();

	virtual UHTNTask* GetTaskByTag( FGameplayTag& TaskTag );

#pragma endregion Task Planning
	
	virtual void OnTaskFinished( FGameplayTag& FinishedTaskTag );

	virtual FGameplayTag GetTaskTagToActive();

	// If something was wrong by external factor.
	virtual void ResetAllTaskTag()
	{
		TaskTagsToActive.Empty();
	};

	// We need to check running plan because of external factor.
	virtual UHTNTask* GetFirstTaskInPlan();

	virtual UHTNTask* GetMatchPreconditionTask();

	virtual void ShuffleTagArray( TArray<FGameplayTag>& ShuffleArray );

	virtual bool DoDepthSearch(FGameplayTag TaskSearchTag, TArray<FGameplayTag>& TaskSequence);

#pragma region Task World State
	
	virtual void AddTaskWorldState(UTaskWorldState* WorldState);

	virtual void AddTaskWorldStateByClass(TSubclassOf<UTaskWorldState> WorldStateClass);

	virtual void RemoveTaskWorldState(TSubclassOf<UTaskWorldState> WorldStateClass);

	virtual void ResetAllTaskWorldStates();
	
#pragma endregion Task World State

	// World Property Getter
	virtual int32 GetWorldStateProperty_Int( FName PropertyName );
	virtual bool GetWorldStateProperty_Bool( FName PropertyName );
	virtual float GetWorldStateProperty_Float( FName PropertyName );

	virtual int32 GetSimulatedProperty_Int( FName PropertyName );
	virtual bool GetSimulatedProperty_Bool( FName PropertyName );
	virtual float GetSimulatedProperty_Float( FName PropertyName );

	virtual void AddWorldSimulatedProperty_Float( FName PropertyName, float Value, bool IsAdded );
	virtual void AddWorldSimulatedProperty_Int( FName PropertyName, int32 Value, bool IsAdded );
	virtual void AddWorldSimulatedProperty_Bool( FName PropertyName, bool Value, bool IsAdded );
	
protected :
	
	UPROPERTY(VisibleAnywhere, Category = "HTN Task | Task To Active")
	TArray<FGameplayTag> TaskTagsToActive;
	
	UPROPERTY(VisibleAnywhere, Category = "HTN Task | Tag To Allocate")
	TMap<FGameplayTag, UHTNTask*> RegisteredTask;
	
	UPROPERTY()
	TArray<TObjectPtr<class UTaskWorldState>> SpawnedTaskWorldStates;
};

