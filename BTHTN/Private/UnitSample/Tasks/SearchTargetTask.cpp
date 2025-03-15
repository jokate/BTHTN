// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/Tasks/SearchTargetTask.h"
#include "HTNModule/HTNBTComponent.h"

void USearchTargetTask::Action()
{
	UE_LOG(LogTemp, Log, TEXT("Finding Target..."));
	Super::Action();
}