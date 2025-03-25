// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/TaskWorldState.h"

void UTaskWorldState::BeginDestroy()
{
	TaskWorldState.ResetAllState();
	TaskWorldStateDelta.ResetAllState();

	UE_LOG(LogTemp, Log, TEXT("WorldState Destroyed"));

	for (TFieldIterator<FProperty> PropIt(GetClass(), EFieldIterationFlags::IncludeSuper); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			if (FTaskRelatedValue* RelatedValue = StructProperty->ContainerPtrToValuePtr<FTaskRelatedValue>(this))
			{
				RelatedValue->SetOwner(nullptr);  // 명시적으로 초기화
			}
		}
	}
	
	Super::BeginDestroy();
}

void UTaskWorldState::AddWorldStateIntegerValue(FName KeyName, int32 Value)
{
	TaskWorldState.AddIntegerValue(KeyName, Value);
}

void UTaskWorldState::AddWorldStateFloatValue(FName KeyName, float Value)
{
	TaskWorldState.AddFloatValue(KeyName, Value);
}

void UTaskWorldState::AddWorldStateBooleanValue(FName KeyName, bool Value)
{
	TaskWorldState.AddBooleanValue( KeyName, Value );
}

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

bool UTaskWorldState::IsPropertyDefined(FName PropertyName) const
{
	return TaskWorldStateDelta.IsPropertyDefined(PropertyName);
}

void UTaskWorldState::AddWorldStateSimulateIntegerValue(FName KeyName)
{
	TaskWorldStateDelta.AddIntegerValue( KeyName, 0 );
}

void UTaskWorldState::AddWorldStateSimulateFloatValue(FName KeyName )
{
	TaskWorldStateDelta.AddFloatValue( KeyName, 0.f );
}

void UTaskWorldState::AddWorldStateSimulateBooleanValue( FName KeyName )
{
	TaskWorldStateDelta.AddBooleanValue(KeyName, false);
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
bool UTaskWorldState::GetWorldSimulateBooleanValue(FName WorldStateName, bool& RetVal)
{
	if ( TaskWorldStateDelta.GetWorldStateBooleanValue( WorldStateName, RetVal ) == false)
	{
		return false;
	}
	
	return true;
}

bool UTaskWorldState::GetWorldSimulateIntegerValue(FName WorldStateName, int32& RetVal)
{
	int32 OriginalValue = 0;
	if ( GetWorldStateIntegerValue(WorldStateName, OriginalValue ) == false )
	{
		return false;
	}

	int32 DeltaValue = 0;
	if ( TaskWorldStateDelta.GetWorldStateIntegerValue(WorldStateName, DeltaValue) == false )
	{
		return false;
	}
	
	RetVal = OriginalValue + DeltaValue;

	return true;
}

bool UTaskWorldState::GetWorldSimulateFloatValue(FName WorldStateName, float& RetVal)
{
	float OriginalValue = 0;
	if ( GetWorldStateFloatValue(WorldStateName, OriginalValue ) == false )
	{
		return false;
	}

	float DeltaValue = 0;
	if ( TaskWorldStateDelta.GetWorldStateFloatValue(WorldStateName, DeltaValue) == false )
	{
		return false;
	}
	
	RetVal = OriginalValue + DeltaValue;
	
	return true;
}

void UTaskWorldState::SetupStructProperties()
{
	for (TFieldIterator<FProperty> PropIt(GetClass(), EFieldIterationFlags::IncludeSuper); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		if (Property == nullptr)
		{
			continue;
		}
		
		FStructProperty* StructProperty = CastField<FStructProperty>(Property);

		if ( StructProperty == nullptr )
		{
			continue;
		}
	
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
				
				AddWorldStateIntegerValue(IntValue->GetKey(), IntValue->GetIntegerValue());
				AddWorldStateSimulateIntegerValue( IntValue->GetKey() );
			}
			break;

		case EHTNTaskRelatedValueType::FLOAT :
			{
				const FTaskRelatedValue_Float* FloatValue = static_cast<FTaskRelatedValue_Float*>(RelatedValue);
				if ( FloatValue == nullptr )
				{
					continue;
				}

				AddWorldStateFloatValue(FloatValue->GetKey(), FloatValue->GetFloatValue());
				AddWorldStateSimulateFloatValue(FloatValue->GetKey());
			}
			break;
		case EHTNTaskRelatedValueType::BOOL :
			{
				const FTaskRelatedValue_Boolean* BoolValue = static_cast<FTaskRelatedValue_Boolean*>(RelatedValue);
				if ( BoolValue == nullptr )
				{
					continue;
				}

				AddWorldStateBooleanValue(BoolValue->GetKey(), BoolValue->GetBooleanValue());
				AddWorldStateSimulateBooleanValue(BoolValue->GetKey());
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
