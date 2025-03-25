// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNBTComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HTNBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
class UHTNBTComponent;
class UTaskWorldState;

UCLASS()
class BTHTN_API UHTNBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	static int32 GetWorldStateProperty_Int( UHTNBTComponent* BTComponent, FName PropertyName );
	static bool GetWorldStateProperty_Bool( UHTNBTComponent* BTComponent, FName PropertyName );
	static float GetWorldStateProperty_Float( UHTNBTComponent* BTComponent, FName PropertyName );

	static int32 GetSimulatedProperty_Int( UHTNBTComponent* BTComponent, FName PropertyName );
	static bool GetSimulatedProperty_Bool( UHTNBTComponent* BTComponent, FName PropertyName );
	static float GetSimulatedProperty_Float( UHTNBTComponent* BTComponent, FName PropertyName );

	static void AddWorldSimulatedProperty_Float( UHTNBTComponent* BTComponent, FName PropertyName, float Value, bool IsAdded );
	static void AddWorldSimulatedProperty_Int( UHTNBTComponent* BTComponent, FName PropertyName, int32 Value, bool IsAdded );
	static void AddWorldSimulatedProperty_Bool( UHTNBTComponent* BTComponent, FName PropertyName, bool Value, bool IsAdded );

	static void SetWorldStateProperty_Int( UHTNBTComponent* BTComponent, FName PropertyName, int32 SettingValue );
	static void SetWorldStateProperty_Float( UHTNBTComponent* BTComponent, FName PropertyName, float SettingValue );
	static void SetWorldStateProperty_Boolean( UHTNBTComponent* BTComponent, FName PropertyName, bool SettingValue );
	
	static UTaskWorldState* GetTaskWorldState( UHTNBTComponent* BTComponent );
};
