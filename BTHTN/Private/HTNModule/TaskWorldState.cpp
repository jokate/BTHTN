// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/TaskWorldState.h"

bool UTaskWorldState::UpdateWorldIntegerValue(FName KeyName, int32 UpdatedValue)
{
	if ( TaskWorldState.UpdateIntegerValue(KeyName, UpdatedValue) == false )
	{
		return false;
	}

	if ( OnUpdatedTaskRelatedValue_Integer.IsBound() == true )
	{
		OnUpdatedTaskRelatedValue_Integer.Broadcast(KeyName, UpdatedValue);
	}

	return true;
}

bool UTaskWorldState::UpdateWorldBooleanValue(FName KeyName, bool UpdatedValue)
{
	if (TaskWorldState.UpdateBooleanValue(KeyName, UpdatedValue) == false )
	{
		return false;
	}

	if ( OnUpdatedTaskRelatedValue_Boolean.IsBound() == true )
	{
		OnUpdatedTaskRelatedValue_Boolean.Broadcast(KeyName, UpdatedValue);
	}

	return true;
}

bool UTaskWorldState::UpdateWorldFloatValue(FName KeyName, float UpdatedValue)
{	
	if ( TaskWorldState.UpdateFloatValue(KeyName, UpdatedValue) == false )
	{
		return false;
	}

	if ( OnUpdatedTaskRelatedValue_Float.IsBound() == true )
	{
		OnUpdatedTaskRelatedValue_Float.Broadcast(KeyName, UpdatedValue);
	}

	return true;
}

bool UTaskWorldState::GetWorldStateBooleanValue(FName WorldStateName, bool& RetVal)
{
	if ( TaskWorldState.GetWorldStateBooleanValue(WorldStateName, RetVal) == false )
	{
		return false;
	}

	return true;
}

bool UTaskWorldState::GetWorldStateIntegerValue(FName WorldStateName, int32& RetVal)
{
	if ( TaskWorldState.GetWorldStateIntegerValue(WorldStateName, RetVal) == false )
	{
		return false;
	}

	return true;
}

bool UTaskWorldState::GetWorldStateFloatValue(FName WorldStateName, float& RetVal)
{
	if ( TaskWorldState.GetWorldStateFloatValue(WorldStateName, RetVal) == false )
	{
		return false;
	}

	return true;
}

void UTaskWorldState::UpdateWorldDeltaIntegerValue(FName KeyName, int32 SimulatedValue, bool IsAdded)
{
	int32 CurrentValue = 0;

	TaskWorldStateDelta.GetWorldStateIntegerValue(KeyName, CurrentValue );

	int32 CalculatedValue = IsAdded ? CurrentValue + SimulatedValue : CurrentValue - SimulatedValue;
	TaskWorldStateDelta.UpdateIntegerValue(KeyName, CalculatedValue);
}

// Boolean operate differently
void UTaskWorldState::UpdateWorldDeltaBooleanValue(FName KeyName, bool IsAdded, bool SimulatedValue )
{
	if ( IsAdded == true )
	{
		TaskWorldStateDelta.UpdateBooleanValue(KeyName, SimulatedValue);
	}
	else
	{
		TaskWorldStateDelta.RemoveBooleanValue(KeyName);
	}
}

void UTaskWorldState::UpdateWorldDeltaFloatValue(FName KeyName, float SimulatedValue, bool IsAdded)
{
	float CurrentValue = 0.f;

	TaskWorldStateDelta.GetWorldStateFloatValue(KeyName, CurrentValue);

	float CalculatedValue = IsAdded ? CurrentValue + SimulatedValue : CurrentValue - SimulatedValue;
	TaskWorldStateDelta.UpdateFloatValue(KeyName, CalculatedValue);
}

// boolean operate differently.
bool UTaskWorldState::GetWorldSimulateStateBooleanValue(FName WorldStateName, bool& RetVal)
{
	if ( TaskWorldStateDelta.GetWorldStateBooleanValue( WorldStateName, RetVal) == false)
	{
		return false;
	}
	
	return true;
}

bool UTaskWorldState::GetWorldSimulateIntegerValue(FName WorldStateName, int32& RetVal)
{
	int32 OriginalValue = 0;
	TaskWorldState.GetWorldStateIntegerValue(WorldStateName, OriginalValue);

	int32 DeltaValue = 0;
	TaskWorldStateDelta.GetWorldStateIntegerValue(WorldStateName, DeltaValue);
	
	RetVal = OriginalValue + DeltaValue;

	return true;
}

bool UTaskWorldState::GetWorldSimulateFloatValue(FName WorldStateName, float& RetVal)
{
	float OriginalValue = 0;
	TaskWorldState.GetWorldStateFloatValue(WorldStateName, OriginalValue);

	float DeltaValue = 0;
	TaskWorldStateDelta.GetWorldStateFloatValue(WorldStateName, DeltaValue);
	
	RetVal = OriginalValue + DeltaValue;
	
	return true;
}

void UTaskWorldState::SetupStructProperties()
{
	for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		FStructProperty* StructProperty = CastField<FStructProperty>(Property);

		FTaskRelatedValue* RelatedValue = StructProperty->ContainerPtrToValuePtr<FTaskRelatedValue>(this);

		// In this class we need to define the Task Conditional Value to mark world status.
		if ( RelatedValue == nullptr )
		{
			continue;
		}
		
		RelatedValue->SetOwner( this );
		EHTNTaskRelatedValueType TaskRelatedValueType = RelatedValue->GetValueType();

		switch ( TaskRelatedValueType )
		{
		case EHTNTaskRelatedValueType::INT :
			{
				const FTaskRelatedValue_Int* IntValue = static_cast<FTaskRelatedValue_Int*>(RelatedValue);
				if ( IntValue == nullptr )
				{
					continue;
				}
				
				UpdateWorldIntegerValue(IntValue->GetKey(), IntValue->GetIntegerValue());
			}
			break;

		case EHTNTaskRelatedValueType::FLOAT :
			{
				const FTaskRelatedValue_Float* FloatValue = static_cast<FTaskRelatedValue_Float*>(RelatedValue);
				if ( FloatValue == nullptr )
				{
					continue;
				}

				UpdateWorldFloatValue(FloatValue->GetKey(), FloatValue->GetFloatValue());
			}
			break;
		case EHTNTaskRelatedValueType::BOOL :
			{
				const FTaskRelatedValue_Boolean* BoolValue = static_cast<FTaskRelatedValue_Boolean*>(RelatedValue);
				if ( BoolValue == nullptr )
				{
					continue;
				}

				UpdateWorldBooleanValue(BoolValue->GetKey(), BoolValue->GetBooleanValue());	
			}
			break;
		default :
			{
				UE_LOG(LogTemp, Error, TEXT("Task Related Value Type Is None : InValid."));
			}
			break;
		}
	}
}
