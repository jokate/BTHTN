// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNBTComponent.h"
#include "HTNModule/HTNTask.h"
#include "HTNModule/TaskWorldState.h"
#include "UnitSample/AttackerWorldState.h"


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

void UHTNBTComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (UTaskWorldState* TaskWorldState : SpawnedTaskWorldStates)
	{
		if ( IsValid(TaskWorldState) == true)
		{
			TaskWorldState->ConditionalBeginDestroy();
			TaskWorldState->MarkAsGarbage();
		}
	}

	SpawnedTaskWorldStates.Empty();
	
	for ( TPair<FGameplayTag, UHTNTask*> HTNTask : RegisteredTask )
	{
		UHTNTask* Task = HTNTask.Value;

		if ( IsValid(Task) == true)
		{
			Task->ConditionalBeginDestroy();
			Task->MarkAsGarbage();
		}
	}

	RegisteredTask.Empty();
	
	Super::EndPlay(EndPlayReason);
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
	TempTaskGameplayTag.Add(RootGameplayTag);
	while (TempTaskGameplayTag.IsEmpty() == false)
	{
		FGameplayTag TaskGameplayTag = TempTaskGameplayTag.Pop();

		UHTNTask* HTNTask = GetTaskByTag(TaskGameplayTag);

		if ( IsValid(HTNTask) == false)
		{
			continue;	
		}

		if ( HTNTask->CheckPrecondition() == false )
		{
			continue;
		}

		EHTNTaskType HTNTaskType = HTNTask->GetTaskType();

		if ( HTNTaskType == EHTNTaskType::COMPOUND )
		{
			
		}
		else
		{
			
		}
	}
	
	TaskTagsToActive = TempTaskGameplayTag;
}


UHTNTask* UHTNBTComponent::GetTaskByTag(FGameplayTag& TaskTag)
{
	for ( TPair<FGameplayTag, UHTNTask*> Task : RegisteredTask )
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

void UHTNBTComponent::RemoveFirstTagInTaskList()
{
	if ( TaskTagsToActive.IsValidIndex(0) == true )
	{
		TaskTagsToActive.RemoveAt(0);
	}
}

void UHTNBTComponent::ShuffleTagArray(TArray<FGameplayTag>& ShuffleArray)
{
	if ( ShuffleArray.Num() > 0 )
	{
		int32 LastIndex = ShuffleArray.Num() - 1;

		for ( int32 i = 0; i <= LastIndex; i++ )
		{
			int32 Index = FMath::RandRange(0, LastIndex);

			if ( i != Index)
			{
				ShuffleArray.Swap(i, Index);
			}
		}
	}
}

void UHTNBTComponent::AddTaskWorldState(UTaskWorldState* WorldState)
{
	if ( IsValid(WorldState) == true )
	{
		WorldState->SetupStructProperties();
		SpawnedTaskWorldStates.Add(WorldState);
	}
}

void UHTNBTComponent::AddTaskWorldStateByClass(TSubclassOf<UTaskWorldState> WorldStateClass)
{
	UTaskWorldState* WorldState = NewObject<UTaskWorldState>(this, WorldStateClass);

	if ( IsValid(WorldState) == true )
	{
		Cast<UAttackerWorldState>(WorldState)->SetupStructProperties();
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
