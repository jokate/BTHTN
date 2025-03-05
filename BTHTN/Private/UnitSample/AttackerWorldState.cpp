// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/AttackerWorldState.h"

UAttackerWorldState::UAttackerWorldState()
{
	TargetFound = FTaskRelatedValue_Boolean("HasTarget", false);
	CurrentHp = FTaskRelatedValue_Int("CurrentHp", 100);
}
