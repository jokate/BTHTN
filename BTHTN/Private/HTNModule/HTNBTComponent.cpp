﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNBTComponent.h"
#include "HTNModule/HTNTask.h"


// Sets default values for this component's properties
UHTNBTComponent::UHTNBTComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHTNBTComponent::BeginPlay()
{
	Super::BeginPlay();

	for ( UHTNTask* RegisteredHTNTask :  RegisteredTask )
	{
		//init owner information to component
		RegisteredHTNTask->InitializeHTNComponent( this );
	}
}


// Called every frame
void UHTNBTComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Task need to be planned instantly when task tag container is empty.
	SimulatePlanningTask();
}

// Simulate Plan.
// 
void UHTNBTComponent::SimulatePlanningTask()
{
	//If Task Already Planned, we need to wait all task is finished.
	if ( TaskTagsToActive.Num() > 0)
	{
		return;
	}
	
	TArray<FGameplayTag> TempTaskGameplayTag;

	// Search Function.

	TaskTagsToActive = TempTaskGameplayTag;
}

UHTNTask* UHTNBTComponent::GetTaskByTag(FGameplayTag& TaskTag)
{
	for ( UHTNTask* Task : RegisteredTask)
	{
		if ( Task->IsTagForTask(TaskTag) == true )
		{
			return Task;
		}
	}

	return nullptr;
}

//if task is finished we need to remove the tag from the list.
void UHTNBTComponent::OnTaskFinished(FGameplayTag& FinishedTaskTag)
{
	if ( TaskTagsToActive.Contains(FinishedTaskTag) == true )
	{
		TaskTagsToActive.Remove(FinishedTaskTag);	
	}
}

FGameplayTag UHTNBTComponent::GetTaskTagToActive()
{
	if ( TaskTagsToActive.IsValidIndex(0) == true )
	{
		return TaskTagsToActive[0];
	}

	// if no gameplay Tag to active. it will be calculating task.
	return FGameplayTag();
}

//Need To Use Behavior Tree Service for check precondition of task.
UHTNTask* UHTNBTComponent::GetFirstTaskInPlan()
{
	FGameplayTag FirstTagToActive = GetTaskTagToActive();
	return GetTaskByTag(FirstTagToActive);
}

