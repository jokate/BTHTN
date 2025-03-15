// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNTask.h"
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
	for ( FTaskSimulateValue& SimulateValue : SimulationValues )
	{
		FName TypeName = SimulateValue.TypeName;
		switch (SimulateValue.TaskRelatedValue)
		{
		case EHTNTaskRelatedValueType::INT :
			{
				int32 CurrentSimulateValue = HTNBTComponent->GetSimulatedProperty_Int(TypeName);
				RetVal &= SimulateValue.CompareValue(CurrentSimulateValue);
			}
			break;
		case EHTNTaskRelatedValueType::BOOL :
			{
				float CurrentSimulateValue = HTNBTComponent->GetSimulatedProperty_Bool(TypeName);
				RetVal &= SimulateValue.CompareValue(CurrentSimulateValue);
			}
			break;
		case EHTNTaskRelatedValueType::FLOAT :
			{
				float CurrentSimulateValue = HTNBTComponent->GetSimulatedProperty_Float(TypeName);
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

AActor* UHTNTask::GetOwner() const
{
	return OwnerActor.Get();
}

UHTNBTComponent* UHTNTask::GetHTNBTComponent() const
{
	return OwnerBTComponent.Get();
}