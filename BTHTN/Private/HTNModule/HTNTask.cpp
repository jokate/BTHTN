// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNTask.h"

#include "HTNModule/HTNBlueprintFunctionLibrary.h"
#include "HTNModule/HTNBTComponent.h"

bool UHTNTask::CheckPrecondition()
{
	UHTNBTComponent* HTNBTComponent = GetHTNBTComponent();

	if ( IsValid(HTNBTComponent) == false )
	{
		return false;
	}
	
	bool RetVal = false;
	// Check World State Value.
	for ( FTaskSimulateValue_Compare& SimulateValue : SimulationValues )
	{
		FName TypeName = SimulateValue.TypeName;
		switch (SimulateValue.TaskRelatedValue)
		{
		case EHTNTaskRelatedValueType::INT :
			{
				int32 CurrentSimulateValue = UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Int(HTNBTComponent, TypeName);
				RetVal &= SimulateValue.CompareValue(CurrentSimulateValue);
			}
			break;
		case EHTNTaskRelatedValueType::BOOL :
			{
				float CurrentSimulateValue = UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Bool(HTNBTComponent, TypeName);
				RetVal &= SimulateValue.CompareValue(CurrentSimulateValue);
			}
			break;
		case EHTNTaskRelatedValueType::FLOAT :
			{
				float CurrentSimulateValue = UHTNBlueprintFunctionLibrary::GetSimulatedProperty_Float(HTNBTComponent, TypeName);
				RetVal &= SimulateValue.CompareValue(CurrentSimulateValue);
			}
			break;
		default :
			{
				// If There is improper value in Simulate Value need to make crash. 
				check(TEXT("INVALID Task Related Value!"));
				return false;
			}
			break;
		}
	}
	return RetVal;
}

void UHTNTask::InitializeHTNComponent(UHTNBTComponent* BTComponent)
{
	// Need To Allocate To Owner of Component.
	if (IsValid(BTComponent) == true)
	{
		OwnerBTComponent = BTComponent;
		
		AActor* ComponentOwnerActor = BTComponent->GetOwner();

		if ( IsValid(ComponentOwnerActor) == true)
		{
			OwnerActor = ComponentOwnerActor;
		}
 	}
}

void UHTNTask::SimulateEffectToOwner( bool bIsAdded )
{
	for ( FTaskSimulateValue& SimulateValue : ApplySimulateValues )
	{
		ApplyTaskSimulateValue( SimulateValue, bIsAdded );
	}
}

AActor* UHTNTask::GetOwner() const
{
	return OwnerActor.Get();
}

UHTNBTComponent* UHTNTask::GetHTNBTComponent() const
{
	return OwnerBTComponent.Get();
}

void UHTNTask::ApplyTaskSimulateValue(FTaskSimulateValue& TaskSimulateValue, bool IsAdded)
{
	UHTNBTComponent* HTNBTComponent = GetHTNBTComponent();

	if ( IsValid(HTNBTComponent) == false )
	{
		return;
	}

	FName TypeName = TaskSimulateValue.TypeName;
	switch (TaskSimulateValue.TaskRelatedValue )
	{
	case EHTNTaskRelatedValueType::INT :
		UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Int( HTNBTComponent, TypeName, TaskSimulateValue.IntValue, IsAdded);
		break;
	case EHTNTaskRelatedValueType::BOOL :
		UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Bool( HTNBTComponent, TypeName, TaskSimulateValue.BoolValue, IsAdded);
		break;
	case EHTNTaskRelatedValueType::FLOAT :
		UHTNBlueprintFunctionLibrary::AddWorldSimulatedProperty_Float( HTNBTComponent, TypeName, TaskSimulateValue.FloatValue, IsAdded);
		break;
	default:
		check("INVALID TYPE");
		break;
	}
}
