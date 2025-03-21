// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNBlueprintFunctionLibrary.h"
#include "HTNModule/HTNBTComponent.h"
#include "HTNModule/TaskWorldState.h"

int32 UHTNBlueprintFunctionLibrary::GetWorldStateProperty_Int(UHTNBTComponent* BTComponent, FName PropertyName)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);

	int32 RetVal = -1;
	for ( UTaskWorldState* WorldState : WorldStates )
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

bool UHTNBlueprintFunctionLibrary::GetWorldStateProperty_Bool(UHTNBTComponent* BTComponent, FName PropertyName)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);
	
	bool RetVal = false;
	
	for ( UTaskWorldState* WorldState : WorldStates )
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

float UHTNBlueprintFunctionLibrary::GetWorldStateProperty_Float(UHTNBTComponent* BTComponent, FName PropertyName)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);
	
	float RetVal = FLT_MAX;
	
	for ( UTaskWorldState* WorldState : WorldStates )
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

int32 UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Int(UHTNBTComponent* BTComponent, FName PropertyName)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);
	
	int32 RetVal = -1;
	for ( UTaskWorldState* WorldState : WorldStates )
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

bool UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Bool(UHTNBTComponent* BTComponent, FName PropertyName)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);
	
	bool RetVal = false;
	
	for ( UTaskWorldState* WorldState : WorldStates )
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

float UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Float(UHTNBTComponent* BTComponent, FName PropertyName)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);
	
	float RetVal = FLT_MAX;
	
	for ( UTaskWorldState* WorldState : WorldStates )
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

void UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Float(UHTNBTComponent* BTComponent, FName PropertyName,
	float Value, bool IsAdded)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);
	
	for ( UTaskWorldState* WorldState : WorldStates )
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

void UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Int(UHTNBTComponent* BTComponent, FName PropertyName,
	int32 Value, bool IsAdded)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);

	for ( UTaskWorldState* WorldState : WorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}

		if ( WorldState->IsPropertyDefined(PropertyName) == true )
		{
			WorldState->UpdateWorldDeltaIntegerValue( PropertyName, Value, IsAdded );
			break;
		}
	}
}

void UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Bool(UHTNBTComponent* BTComponent, FName PropertyName,
	bool Value, bool IsAdded)
{
	TArray<TObjectPtr<UTaskWorldState>>& WorldStates = GetTaskWorldState(BTComponent);

	for ( UTaskWorldState* WorldState : WorldStates )
	{
		if ( IsValid(WorldState) == false )
		{
			continue;
		}

		if ( WorldState->IsPropertyDefined(PropertyName) == true )
		{
			WorldState->UpdateWorldDeltaBooleanValue( PropertyName, Value, IsAdded );
			break;
		}
	}
}

//if BTComponent Is not valid crash.
TArray<TObjectPtr<UTaskWorldState>>& UHTNBlueprintFunctionLibrary::GetTaskWorldState(UHTNBTComponent* BTComponent)
{
	check(BTComponent);
	return BTComponent->GetTaskWorldStates();
}
