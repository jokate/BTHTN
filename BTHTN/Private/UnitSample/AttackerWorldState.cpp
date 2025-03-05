// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/AttackerWorldState.h"

UAttackerWorldState::UAttackerWorldState()
{
	TargetFound = FTaskRelatedValue_Boolean("HasTarget", false);
	CurrentHp = FTaskRelatedValue_Int("CurrentHp", 100);
}

void UAttackerWorldState::UpdateWorldBooleanValue(FName KeyName, bool UpdatedValue)
{
	Super::UpdateWorldBooleanValue(KeyName, UpdatedValue);
}

void UAttackerWorldState::UpdateWorldFloatValue(FName KeyName, float UpdatedValue)
{
	Super::UpdateWorldFloatValue(KeyName, UpdatedValue);
}

void UAttackerWorldState::UpdateWorldIntegerValue(FName KeyName, int32 UpdatedValue)
{
	Super::UpdateWorldIntegerValue(KeyName, UpdatedValue);
}
