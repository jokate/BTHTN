// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTNModule/TaskWorldState.h"
#include "AttackerWorldState.generated.h"

/**
 * 
 */
UCLASS()
class BTHTN_API UAttackerWorldState : public UTaskWorldState
{
	GENERATED_BODY()

public :
	UAttackerWorldState();
	
	UPROPERTY(BlueprintReadWrite, Category = "Attacker World")
	FTaskRelatedValue_Boolean TargetFound;

	UPROPERTY(BlueprintReadWrite, Category = "Attacker World")
	FTaskRelatedValue_Int CurrentHp;
};
