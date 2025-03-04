﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNBTComponent.h"
#include "HTNModule/HTNTask.h"
#include "HTNModule/TaskWorldState.h"


// Sets default values for this component's properties
UHTNBTComponent::UHTNBTComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHTNBTComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHTNBTComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Task need to be planned instantly when task tag container is empty.
	SimulatePlanningTask();
}

void UHTNBTComponent::RegisterTask(TSubclassOf<UHTNTask> HTNTaskClass )
{
	AActor* Owner = GetOwner();

	if ( IsValid(Owner) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UHTNBTComponent::RegisterTask : Owner Is Not Valid"));
		return;
	}

	UHTNTask* HTNTask = NewObject<UHTNTask>(Owner, HTNTaskClass);

	if ( IsValid(HTNTask) == false )
	{
		UE_LOG(LogTemp, Error, TEXT("UHTNBTComponent::RegisterTask : Task Creation Failed"));
		return;
	}
	
	if ( RegisteredTask.Contains(HTNTask->GetTaskTag()) == true )
	{
		UE_LOG(LogTemp, Error, TEXT("UHTNBTComponent::RegisterTask : Task Tag Is Alreay Defined"));
		HTNTask->BeginDestroy();
		return;
	}
	HTNTask->InitializeHTNComponent(this);
	RegisteredTask.Add(HTNTask->GetTaskTag(), HTNTask);
}

// Simulate Plan.
// 
void UHTNBTComponent::SimulatePlanningTask()
{
	//If Task Already Planned, we need to wait all task is finished.
	if ( TaskTagsToActive.Num() > 0)
	{
		return;
	}
	
	TArray<FGameplayTag> TempTaskGameplayTag;

	// Search Function.

	TaskTagsToActive = TempTaskGameplayTag;
}

UHTNTask* UHTNBTComponent::GetTaskByTag(FGameplayTag& TaskTag)
{
	for ( TPair<FGameplayTag, UHTNTask*> Task : RegisteredTask)
	{
		if ( Task.Key == TaskTag )
		{
			return Task.Value;
		}
	}

	return nullptr;
}

//if task is finished we need to remove the tag from the list.
void UHTNBTComponent::OnTaskFinished(FGameplayTag& FinishedTaskTag)
{
	if ( TaskTagsToActive.Contains(FinishedTaskTag) == true )
	{
		TaskTagsToActive.Remove(FinishedTaskTag);	
	}
}

FGameplayTag UHTNBTComponent::GetTaskTagToActive()
{
	if ( TaskTagsToActive.IsValidIndex(0) == true )
	{
		return TaskTagsToActive[0];
	}

	// if no gameplay Tag to active. it will be calculating task.
	return FGameplayTag();
}

//Need To Use Behavior Tree Service for check precondition of task.
UHTNTask* UHTNBTComponent::GetFirstTaskInPlan()
{
	FGameplayTag FirstTagToActive = GetTaskTagToActive();
	return GetTaskByTag(FirstTagToActive);
}

void UHTNBTComponent::AddTaskWorldState(UTaskWorldState* WorldState)
{
	if ( IsValid(WorldState) == true )
	{
		SpawnedTaskWorldStates.Add(WorldState);
	}
}

void UHTNBTComponent::AddTaskWorldStateByClass(TSubclassOf<UTaskWorldState> WorldStateClass)
{
	UTaskWorldState* WorldState = NewObject<UTaskWorldState>(WorldStateClass);

	if ( IsValid(WorldState) == true )
	{
		SpawnedTaskWorldStates.Add(WorldState);
	}
}

void UHTNBTComponent::RemoveTaskWorldState(TSubclassOf<UTaskWorldState> WorldStateClass)
{
	UTaskWorldState* NeedToRemove = nullptr;
	
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if (WorldState->GetClass() == WorldStateClass)
		{
			NeedToRemove = WorldState;
			break;
		}
	}

	if ( IsValid(NeedToRemove) == true )
	{
		SpawnedTaskWorldStates.Remove(NeedToRemove);	
	}
}

void UHTNBTComponent::ResetAllTaskWorldStates()
{
	SpawnedTaskWorldStates.Empty();
}

int32 UHTNBTComponent::GetWorldStateProperty_Int(FName PropertyName)
{
	int32 RetVal = -1;
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}
		
		if (WorldState->GetWorldStateIntegerValue(PropertyName, RetVal) == true)
		{
			break;
		}
	}

	return RetVal;
}

// Caution : Boolean Value Can be false by Default Value. So you NEED TO be careful when you are handling boolean Property.
bool UHTNBTComponent::GetWorldStateProperty_Bool(FName PropertyName)
{
	bool RetVal = false;
	
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}
		
		if (WorldState->GetWorldStateBooleanValue(PropertyName, RetVal) == true)
		{
			break;
		}
	}

	return RetVal;
}


float UHTNBTComponent::GetWorldStateProperty_Float(FName PropertyName)
{
	float RetVal = FLT_MAX;
	
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}
		
		if (WorldState->GetWorldStateFloatValue(PropertyName, RetVal) == true)
		{
			break;
		}
	}

	return RetVal;
}
