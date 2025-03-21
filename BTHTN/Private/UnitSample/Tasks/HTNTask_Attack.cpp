// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/Tasks/HTNTask_Attack.h"

void UHTNTask_Attack::TestActionLog()
{
	Super::TestActionLog();

	UE_LOG(HTNTaskLog, Log, TEXT("Test Log : Attack!"));
}
