// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EHTNTaskRelatedValueType : uint8
{
	NONE,
	FLOAT,
	BOOL,
	INT
};

UENUM( BlueprintType )
enum class EHTNTaskCheckType : uint8
{
	NONE	UMETA(DisplayName = "None"),
	GREATER  UMETA(DisplayName = "Greater"),
	GREATER_EQUAL	UMETA(DisplayName = "Greater Equal"),
	LESS	 UMETA(DisplayName = "Less"),
	LESS_EQUAL		UMETA(DisplayName = "Less Equal"),
	EQUAL			UMETA(DisplayName = "Equal"),
};

UENUM( BlueprintType )
enum class EHTNTaskType : uint8
{
	COMPOUND,
	PRIMITIVE,
};