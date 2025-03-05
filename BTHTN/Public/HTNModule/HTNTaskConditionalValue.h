// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/WorldStateInterface.h"
#include "HTNTaskConditionalValue.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EHTNTaskRelatedValueType : uint8
{
	NONE,
	FLOAT,
	BOOL,
	INT
};

USTRUCT(BlueprintType)
struct FTaskRelatedValue
{
  GENERATED_BODY()
public :

	FTaskRelatedValue() {};
	
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
	// We assume that this values will be used to determine action plans.
	// So this world state will be cached sometimes while blackboardasset is not.
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
		if ( WorldState_Boolean.Contains(KeyName) == false)
		{
			return false;
		}
		bool& NeedToUpdateValue = WorldState_Boolean.FindOrAdd(KeyName);
		NeedToUpdateValue = UpdatedValue;

		return true;
	}

	bool UpdateFloatValue( FName KeyName, float UpdatedValue )
	{
		if ( WorldState_Float.Contains(KeyName) == false)
		{
			return false;
		}
		
		float& NeedToUpdateValue = WorldState_Float.FindOrAdd(KeyName);
		NeedToUpdateValue = UpdatedValue;

		return true;
	}

	bool GetWorldStateBooleanValue( FName WorldStateName, bool& RetVal ) const
	{
		if ( WorldState_Boolean.Contains(WorldStateName) == false)
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
	
protected : 
	UPROPERTY( VisibleAnywhere, Category = "INT" )
	TMap<FName, int32> WorldState_Int32;

	UPROPERTY( VisibleAnywhere, Category = "BOOLEAN")
	TMap<FName, bool> WorldState_Boolean;

	UPROPERTY( VisibleAnywhere, Category = "FLOAT")
	TMap<FName, float> WorldState_Float;
};