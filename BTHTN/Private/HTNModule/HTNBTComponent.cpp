// Fill out your copyright notice in the Description page of Project Settings.


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
	
	UHTNTask* Task = GetMatchPreconditionTask();

	if ( IsValid(Task) == false)
	{
		return;
	}

	TArray<FGameplayTag> TempTaskGameplayTag;

	// Search Function.
	// 
	TempTaskGameplayTag.Add(Task->GetTaskTag());
	
	DoDepthSearch(Task->GetTaskTag(), TempTaskGameplayTag);
	
	TaskTagsToActive = TempTaskGameplayTag;
}

bool UHTNBTComponent::DoDepthSearch(FGameplayTag TaskSearchTag, TArray<FGameplayTag>& TaskSequence)
{
	UHTNTask* Task = GetTaskByTag( TaskSearchTag );

	if ( IsValid(Task) == false )
	{
		return false;
	}

	if ( Task->CheckPrecondition() == false )
	{
		return false;
	}

	// Simulate Effect To WorldState
	Task->SimulateEffectToOwner();
	
	TArray<FGameplayTag> NextTags;
	Task->GetPossibleNextTag().GetGameplayTagArray(NextTags);
	ShuffleTagArray(NextTags);

	if ( NextTags.Num() == 0 )
	{
		return true;
	}

	// Search Until the Dead End.
	bool IsSuccess = false;
	for (FGameplayTag& NextTag : NextTags)
	{
		UHTNTask* NextTask = GetTaskByTag(NextTag);
			
		if ( IsValid(NextTask) == true )
		{
			TaskSequence.Add(NextTask->GetTaskTag());
			if ( DoDepthSearch(NextTask->GetTaskTag(), TaskSequence) == true )
			{
				IsSuccess = true;
			 	break;	
			}
			
			TaskSequence.RemoveAt(TaskSequence.Num() - 1);
		}
	}

	if ( IsSuccess == false )
	{
		TaskSequence.RemoveAt(TaskSequence.Num() - 1);
	}

	Task->AfterSimulateEffectToOwner();
	
	return IsSuccess;
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

UHTNTask* UHTNBTComponent::GetMatchPreconditionTask()
{
	UHTNTask* MatchTask = nullptr;
	for ( TPair<FGameplayTag, UHTNTask*> Task : RegisteredTask )
	{
		UHTNTask* TaskValue = Task.Value;

		// if first Match 
		if ( IsValid( TaskValue ) == true && TaskValue->CheckPrecondition() == true )
		{
			MatchTask = TaskValue;
			break;
		}
	}

	return MatchTask;
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

		if ( WorldState->IsPropertyDefined(PropertyName) == false)
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

		if ( WorldState->IsPropertyDefined(PropertyName) == false)
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

		if ( WorldState->IsPropertyDefined(PropertyName) == false)
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

int32 UHTNBTComponent::GetSimulatedProperty_Int(FName PropertyName)
{
	int32 RetVal = -1;
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}
		
		if ( WorldState->IsPropertyDefined(PropertyName) == false)
		{
			continue;
		}
		
		if (WorldState->GetWorldSimulateIntegerValue(PropertyName, RetVal) == true)
		{
			break;
		}
	}

	return RetVal;
}

bool UHTNBTComponent::GetSimulatedProperty_Bool(FName PropertyName)
{
	bool RetVal = false;
	
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}

		if ( WorldState->IsPropertyDefined(PropertyName) == false)
		{
			continue;
		}
		
		if ( WorldState->GetWorldSimulateBooleanValue(PropertyName, RetVal) == true )
		{
			break;
		}
	}

	return RetVal;
}

float UHTNBTComponent::GetSimulatedProperty_Float(FName PropertyName)
{
	float RetVal = FLT_MAX;
	
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}

		if ( WorldState->IsPropertyDefined(PropertyName) == false)
		{
			continue;
		}
		
		if (WorldState->GetWorldSimulateFloatValue(PropertyName, RetVal) == true)
		{
			break;
		}
	}

	return RetVal;
}

// We Assume that World State Already Has Defined Property
void UHTNBTComponent::AddWorldSimulatedProperty_Float(FName PropertyName, float Value, bool IsAdded )
{
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}

		if ( WorldState->IsPropertyDefined(PropertyName) == true )
		{
			WorldState->UpdateWorldDeltaFloatValue(PropertyName, Value, IsAdded );
			break;
		}
	}
}

void UHTNBTComponent::AddWorldSimulatedProperty_Int(FName PropertyName, int32 Value, bool IsAdded )
{
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}

		if ( WorldState->IsPropertyDefined(PropertyName) == true )
		{
			WorldState->UpdateWorldDeltaIntegerValue(PropertyName, Value, IsAdded);
			break;
		}
	}
}

void UHTNBTComponent::AddWorldSimulatedProperty_Bool(FName PropertyName, bool Value, bool IsAdded )
{
	for ( UTaskWorldState* WorldState : SpawnedTaskWorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}

		if ( WorldState->IsPropertyDefined(PropertyName) == true )
		{
			WorldState->UpdateWorldDeltaBooleanValue(PropertyName, IsAdded, Value );
			break;
		}
	}
}

