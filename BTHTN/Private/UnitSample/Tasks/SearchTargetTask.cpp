// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/Tasks/SearchTargetTask.h"
#include "HTNModule/HTNBTComponent.h"

void USearchTargetTask::TestActionLog()
{
	Super::TestActionLog();

	UE_LOG( HTNTaskLog, Log, TEXT("Test Log : Target Search") );
}
