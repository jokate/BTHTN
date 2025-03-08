// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNTaskConditionalValue.h"
#include "Interface/WorldStateInterface.h"
#include "UObject/Object.h"
#include "TaskWorldState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatedTaskRelatedValue_Float, FName, KeyName, float, UpdatedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatedTaskRelatedValue_Boolean, FName, KeyName, bool, UpdatedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatedTaskRelatedValue_Integer, FName, KeyName, int32, UpdatedValue);


UCLASS()
class BTHTN_API UTaskWorldState : public UObject, public IWorldStateInterface
{
	GENERATED_BODY()

public :

	//Need To Update World State Integer Value (Event Callbacks)
	UFUNCTION()
	virtual bool UpdateWorldIntegerValue( FName KeyName, int32 UpdatedValue ) override;

	UFUNCTION()
	virtual bool UpdateWorldBooleanValue( FName KeyName, bool UpdatedValue ) override;

	UFUNCTION()
	virtual bool UpdateWorldFloatValue( FName KeyName, float UpdatedValue ) override;

	//Original Values
	virtual bool GetWorldStateBooleanValue( FName WorldStateName, bool& RetVal );

	virtual bool GetWorldStateIntegerValue( FName WorldStateName, int32& RetVal );

	virtual bool GetWorldStateFloatValue( FName WorldStateName, float& RetVal );

	// Simulation Value.
	UFUNCTION()
	virtual void UpdateWorldDeltaIntegerValue( FName KeyName, int32 SimulatedValue, bool IsAdded );

	UFUNCTION()
	virtual void UpdateWorldDeltaBooleanValue( FName KeyName, bool IsAdded, bool SimulatedValue = false );

	UFUNCTION()
	virtual void UpdateWorldDeltaFloatValue( FName KeyName, float SimulatedValue, bool IsAdded );
	
	virtual bool GetWorldSimulateStateBooleanValue( FName WorldStateName, bool& RetVal );

	virtual bool GetWorldSimulateIntegerValue( FName WorldStateName, int32& RetVal );

	virtual bool GetWorldSimulateFloatValue( FName WorldStateName, float& RetVal );
	
protected :

	//We need to save all data in task related value in this world state
	void SetupStructProperties();
	
protected : 
	UPROPERTY(VisibleAnywhere)
	FTaskWorldStateData TaskWorldState;

	UPROPERTY( VisibleAnywhere )
	FTaskWorldStateData TaskWorldStateDelta;
	
	UPROPERTY( BlueprintAssignable, BlueprintCallable )
	FOnUpdatedTaskRelatedValue_Float OnUpdatedTaskRelatedValue_Float;

	UPROPERTY( BlueprintAssignable, BlueprintCallable )
	FOnUpdatedTaskRelatedValue_Boolean OnUpdatedTaskRelatedValue_Boolean;

	UPROPERTY( BlueprintAssignable, BlueprintCallable )
	FOnUpdatedTaskRelatedValue_Integer OnUpdatedTaskRelatedValue_Integer;
	
};
