// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WorldStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UWorldStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BTHTN_API IWorldStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool UpdateWorldIntegerValue( FName KeyName, int32 UpdatedValue ) { return true; }
	
	virtual bool UpdateWorldBooleanValue( FName KeyName, bool UpdatedValue ) { return true; }
	
	virtual bool UpdateWorldFloatValue( FName KeyName, float UpdatedValue ) { return true; }

	
};
