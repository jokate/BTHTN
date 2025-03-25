// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNBlueprintFunctionLibrary.h"
#include "HTNModule/HTNBTComponent.h"
#include "HTNModule/TaskWorldState.h"

int32 UHTNBlueprintFunctionLibrary::GetWorldStateProperty_Int(UHTNBTComponent* BTComponent, FName PropertyName)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);

	int32 RetVal = -1;
	if ( IsValid(WorldState) == false )
	{
		return -1;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == false)
	{
		return -1;
	}
		
	if (WorldState->GetWorldStateIntegerValue(PropertyName, RetVal) == true)
	{
		return -1;
	}

	return RetVal;
}

bool UHTNBlueprintFunctionLibrary::GetWorldStateProperty_Bool(UHTNBTComponent* BTComponent, FName PropertyName)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);
	
	bool RetVal = false;
	
	if ( IsValid(WorldState) == false )
	{
		return false;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == false)
	{
		return false;
	}
		
	if (WorldState->GetWorldStateBooleanValue(PropertyName, RetVal) == false)
	{
		return false;
	}

	return RetVal;
}

float UHTNBlueprintFunctionLibrary::GetWorldStateProperty_Float(UHTNBTComponent* BTComponent, FName PropertyName)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);
	
	float RetVal = FLT_MAX;
	
	if ( IsValid(WorldState) == false )
	{
		return FLT_MAX;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == false)
	{
		return FLT_MAX;
	}
		
	if ( WorldState->GetWorldStateFloatValue(PropertyName, RetVal) == false )
	{
		return FLT_MAX;
	}

	return RetVal;
}

int32 UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Int(UHTNBTComponent* BTComponent, FName PropertyName)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);
	
	int32 RetVal = -1;
	if ( IsValid(WorldState) == false )
	{
		return -1;
	}
		
	if ( WorldState->IsPropertyDefined(PropertyName) == false)
	{
		return -1;
	}
		
	if (WorldState->GetWorldSimulateIntegerValue(PropertyName, RetVal) == false)
	{
		return -1;
	}

	return RetVal;
}

bool UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Bool(UHTNBTComponent* BTComponent, FName PropertyName)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);
	
	bool RetVal = false;
	
	if ( IsValid(WorldState) == false )
	{
		return false;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == false)
	{
		return false;
	}
		
	if ( WorldState->GetWorldSimulateBooleanValue(PropertyName, RetVal) == false )
	{
		return false;
	}

	return RetVal;
}

float UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Float(UHTNBTComponent* BTComponent, FName PropertyName)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);
	
	float RetVal = FLT_MAX;
	
	if ( IsValid(WorldState) == false )
	{
		return FLT_MAX;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == false)
	{
		return FLT_MAX;
	}
		
	if (WorldState->GetWorldSimulateFloatValue(PropertyName, RetVal) == false)
	{
		return FLT_MAX;
	}

	return RetVal;
}

void UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Float(UHTNBTComponent* BTComponent, FName PropertyName,
	float Value, bool IsAdded)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);
	
	if ( IsValid(WorldState) == false )
	{
		return;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == true )
	{
		WorldState->UpdateWorldDeltaFloatValue(PropertyName, Value, IsAdded );
	}
}

void UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Int(UHTNBTComponent* BTComponent, FName PropertyName,
	int32 Value, bool IsAdded)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);

	if ( IsValid(WorldState) == false )
	{
		return;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == true )
	{
		WorldState->UpdateWorldDeltaIntegerValue( PropertyName, Value, IsAdded );
	}
}

void UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Bool(UHTNBTComponent* BTComponent, FName PropertyName,
	bool Value, bool IsAdded)
{
	UTaskWorldState* WorldState = GetTaskWorldState(BTComponent);

	if ( IsValid(WorldState) == false )
	{
		return;
	}

	if ( WorldState->IsPropertyDefined(PropertyName) == true )
	{
		WorldState->UpdateWorldDeltaBooleanValue( PropertyName, Value, IsAdded );
	}
}

void UHTNBlueprintFunctionLibrary::SetWorldStateProperty_Int(UHTNBTComponent* BTComponent, FName PropertyName,
	int32 SettingValue)
{
	
}

void UHTNBlueprintFunctionLibrary::SetWorldStateProperty_Float(UHTNBTComponent* BTComponent, FName PropertyName,
	float SettingValue)
{
}

void UHTNBlueprintFunctionLibrary::SetWorldStateProperty_Boolean(UHTNBTComponent* BTComponent, FName PropertyName,
	bool SettingValue)
{
}

//if BTComponent Is not valid crash.
UTaskWorldState* UHTNBlueprintFunctionLibrary::GetTaskWorldState(UHTNBTComponent* BTComponent)
{
	check(BTComponent);
	return BTComponent->GetTaskWorldState();
}
