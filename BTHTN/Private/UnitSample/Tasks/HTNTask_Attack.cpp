// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/Tasks/HTNTask_Attack.h"

void UHTNTask_Attack::Action()
{
	Super::Action();

	UE_LOG(LogTemp, Log, TEXT("Attack!"));
}
