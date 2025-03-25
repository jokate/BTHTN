// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNEnum.h"
#include "Interface/WorldStateInterface.h"
#include "HTNTaskConditionalValue.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FTaskRelatedValue
{
	GENERATED_BODY()
public :

	FTaskRelatedValue() {};
	~FTaskRelatedValue() = default;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	FName Key;
	
	TVariant<int32, bool, float> Value;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	EHTNTaskRelatedValueType ValueType = EHTNTaskRelatedValueType::NONE;

	UPROPERTY()
	TWeakObjectPtr<UObject> OwnerWorldState = nullptr;
	
	template<typename T>
	T GetValue() const {
		if (Value.IsType<T>())
		{
			return Value.Get<T>();
		}
		UE_LOG(LogTemp, Warning, TEXT("FTaskRelatedValue: Invalid type access!"));
		return T();
	}

	FName GetKey() const
	{
		return Key;	
	}
	
	template<typename T>
	void UpdateValue( T& UpdateValue );

	void ResetOwner()
	{
		OwnerWorldState.Reset();
	}

	bool operator==(const FTaskRelatedValue& Other) const
	{
		return Other.Key == Key;
	}

	void SetOwner( UObject* WorldState )
	{
		OwnerWorldState = WorldState;
	}

	UObject* GetOwner() const
	{
		return OwnerWorldState.Get();
	}
	
	EHTNTaskRelatedValueType GetValueType() const { return ValueType; }
};

template <typename T>
void FTaskRelatedValue::UpdateValue(T& UpdateValue)
{
	if (Value.IsType<T>() && Value.Get<T>() == UpdateValue)
	{
		return;
	}

	Value.Set<T>(UpdateValue);
}


USTRUCT(BlueprintType)
struct FTaskRelatedValue_Float : public FTaskRelatedValue
{
	GENERATED_BODY()
	FTaskRelatedValue_Float() {};
	~FTaskRelatedValue_Float() = default;
	FTaskRelatedValue_Float( FName InKey, float InValue )
	{
		Key = InKey;
		ValueType = EHTNTaskRelatedValueType::FLOAT;
		Value.Set<float>(InValue);
	}

	float GetFloatValue() const
	{
		return GetValue<float>();
	}
	
	void UpdateFloatValue( float& InValue )
	{
		const float CurValue = GetFloatValue();
		UpdateValue<float>( InValue );
		
		if ( CurValue != InValue )
		{
			UObject* Owner = GetOwner();
			
			if ( IsValid( Owner ) == true )
			{
				if (IWorldStateInterface* WorldState = Cast<IWorldStateInterface>(Owner) )
				{
					WorldState->UpdateWorldFloatValue( GetKey(), InValue );	
				}
			}
		}
	}
};

USTRUCT( BlueprintType )
struct FTaskRelatedValue_Boolean : public FTaskRelatedValue
{
	GENERATED_BODY()
	FTaskRelatedValue_Boolean() {};
	~FTaskRelatedValue_Boolean() = default;
	FTaskRelatedValue_Boolean( FName InKey, bool InValue )
	{
		Key = InKey;
		ValueType = EHTNTaskRelatedValueType::BOOL;
		Value.Set<bool>(InValue);
	}

	bool GetBooleanValue() const
	{
		return GetValue<bool>();		
	}

	void UpdateBoolValue( bool& InValue )
	{
		const bool CurValue = GetBooleanValue();
		UpdateValue<bool>(InValue);

		if ( CurValue != InValue )
		{
			UObject* Owner = GetOwner();
			
			if ( IsValid( Owner ) == true )
			{
				if (IWorldStateInterface* WorldState = Cast<IWorldStateInterface>(Owner) )
				{
					WorldState->UpdateWorldBooleanValue( GetKey(), InValue );
				}
			}
		}
	}
};

USTRUCT( BlueprintType )
struct FTaskRelatedValue_Int : public FTaskRelatedValue
{
	GENERATED_BODY()
	FTaskRelatedValue_Int() {}
	~FTaskRelatedValue_Int() = default;
	FTaskRelatedValue_Int( FName InKey, int32 InValue )
	{
		Key = InKey;
		ValueType = EHTNTaskRelatedValueType::INT;
		Value.Set<int32>(InValue);
	}
	
	int32 GetIntegerValue() const
	{
		return GetValue<int32>();
	}

	void UpdateIntegerValue( int32& InValue )
	{
		const int32 CurValue = GetIntegerValue();
		UpdateValue<int32>(InValue);

		if ( CurValue != InValue )
		{
			UObject* Owner = GetOwner();
			
			if ( IsValid( Owner ) == true )
			{
				if (IWorldStateInterface* WorldState = Cast<IWorldStateInterface>(Owner) )
				{
					WorldState->UpdateWorldIntegerValue( GetKey(), InValue );
				}
			}
		}
	}
};

USTRUCT(BlueprintType)
struct FTaskWorldStateData
{
	GENERATED_BODY()

public :
	void ResetAllState()
	{
		WorldState_Boolean.Empty();
		WorldState_Float.Empty();
		WorldState_Int32.Empty();
	};
	// We assume that this values will be used to determine action plans.
	// So this world state will be cached sometimes while blackboardasset is not.
	void AddIntegerValue( FName KeyName, int32 InValue )
	{
		WorldState_Int32.Add(KeyName, InValue);
	}

	void AddFloatValue( FName KeyName, float InValue)
	{
		WorldState_Float.Add(KeyName, InValue);
	}

	void AddBooleanValue( FName KeyName, bool InValue )
	{
		WorldState_Boolean.Add(KeyName, InValue);
	}
	
	bool UpdateIntegerValue( FName KeyName, int32 UpdatedValue)
	{
		if ( WorldState_Int32.Contains(KeyName) == false)
		{
			return false;	
		}
		
		int32& NeedToUpdateValue = WorldState_Int32.FindOrAdd(KeyName);
		NeedToUpdateValue = UpdatedValue;

		return true;
	}

	bool UpdateBooleanValue( FName KeyName, bool UpdatedValue)
	{
		if ( WorldState_Boolean.Contains(KeyName) == false )
		{
			return false;
		}
		
		bool& NeedToUpdateValue = WorldState_Boolean.FindOrAdd(KeyName);
		NeedToUpdateValue = UpdatedValue;

		return true;
	}

	bool UpdateFloatValue( FName KeyName, float UpdatedValue )
	{
		if ( WorldState_Boolean.Contains(KeyName) == false )
        {
        	return false;
        }

		float& NeedToUpdateValue = WorldState_Float.FindOrAdd(KeyName);
		NeedToUpdateValue = UpdatedValue;

		return true;
	}

	bool GetWorldStateBooleanValue( FName WorldStateName, bool& RetVal ) const
	{
		if ( WorldState_Boolean.Contains( WorldStateName ) == false )
		{
			return false;
		}
		
		RetVal =  WorldState_Boolean[WorldStateName];
		return true;
	}

	bool GetWorldStateIntegerValue( FName WorldStateName, int32& RetVal ) const
	{
		if ( WorldState_Int32.Contains(WorldStateName) == false )
		{
			return false;
		}
		
		RetVal = WorldState_Int32[WorldStateName];
		return true;
	};

	bool GetWorldStateFloatValue( FName WorldStateName, float& RetVal ) const
	{
		if ( WorldState_Float.Contains(WorldStateName) == false )
		{
			return false;
		}
		
		RetVal = WorldState_Float[WorldStateName];
		return true;
	}

	void RemoveBooleanValue( FName KeyName )
	{
		WorldState_Boolean.Remove(KeyName);
	}

	bool IsPropertyDefined( FName KeyName ) const
	{
		return WorldState_Int32.Contains( KeyName )
		|| WorldState_Float.Contains( KeyName )
		|| WorldState_Boolean.Contains( KeyName );
	}
	
protected : 
	UPROPERTY( VisibleAnywhere, Category = "INT" )
	TMap<FName, int32> WorldState_Int32;

	UPROPERTY( VisibleAnywhere, Category = "BOOLEAN")
	TMap<FName, bool> WorldState_Boolean;

	UPROPERTY( VisibleAnywhere, Category = "FLOAT")
	TMap<FName, float> WorldState_Float;
};

// Purpose
// Compare Simulate World Value.
USTRUCT( BlueprintType )
struct FTaskSimulateValue
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere )
	EHTNTaskRelatedValueType TaskRelatedValue = EHTNTaskRelatedValueType::NONE;

	UPROPERTY( EditAnywhere )
	FName TypeName;
	
	UPROPERTY( EditAnywhere, meta = (EditCondition = "TaskRelatedValue == EHTNTaskRelatedValueType::FLOAT", EditConditionHides))
	float FloatValue;

	UPROPERTY( EditAnywhere, meta = (EditCondition = "TaskRelatedValue == EHTNTaskRelatedValueType::INT", EditConditionHides))
	int32 IntValue;

	UPROPERTY( EditAnywhere, meta = (EditCondition = "TaskRelatedValue == EHTNTaskRelatedValueType::BOOL", EditConditionHides))
	bool BoolValue;
};

USTRUCT( BlueprintType )
struct FTaskSimulateValue_Compare : public FTaskSimulateValue
{
	GENERATED_BODY()
public :
	template<typename T>
	bool CompareValue( T& IncomeValue )
	{
		TFunctionRef<bool(T, T)> Operator = GetOperatorFunction<T>();
		switch(TaskRelatedValue)
		{
		case EHTNTaskRelatedValueType::INT :
			return Operator(IntValue, IncomeValue );
		case EHTNTaskRelatedValueType::FLOAT :
			return Operator(FloatValue, IncomeValue);
		case EHTNTaskRelatedValueType::BOOL :
			return Operator(BoolValue, IncomeValue);
			
		case EHTNTaskRelatedValueType::NONE: break;
		}

		return false;
	}

private : 
	template<typename T>
	TFunctionRef<bool(T, T)> GetOperatorFunction()
	{
		switch (TaskCheckType)
		{
		case EHTNTaskCheckType::LESS :
			return [](T Prev, T After) { return Prev > After; };
		case EHTNTaskCheckType::EQUAL :
			return [](T Prev, T After) { return Prev == After; };
		case EHTNTaskCheckType::GREATER :
			return [](T Prev, T After) { return Prev < After; };
		case EHTNTaskCheckType::LESS_EQUAL :
			return [](T Prev, T After) { return Prev >= After; };
		case EHTNTaskCheckType::GREATER_EQUAL :
			return [](T Prev, T After ) { return Prev <= After; };
		default :
			{
				check("INVALID TYPE");
			}
		}

		return [](T Prev, T After) { return false; };
	}
	
public :
	UPROPERTY( EditAnywhere, meta = (EditCondition = "TaskRelatedValue != EHTNTaskRelatedValueType::NONE", EditConditionHides)  )
	EHTNTaskCheckType TaskCheckType = EHTNTaskCheckType::NONE;
};
