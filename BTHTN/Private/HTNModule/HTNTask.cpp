// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNTask.h"
#include "HTNModule/HTNBTComponent.h"

void UHTNTask::InitializeHTNComponent(UHTNBTComponent* BTComponent)
{
	// Need To Allocate To Owner of Component.
	if (IsValid(BTComponent) == true)
	{
		OwnerBTComponent = BTComponent;
		
		AActor* ComponentOwnerActor = BTComponent->GetOwner();

		if ( IsValid(ComponentOwnerActor) == true)
		{
			OwnerActor = ComponentOwnerActor;
		}
 	}
}

AActor* UHTNTask::GetOwner() const
{
	return OwnerActor.Get();
}

UHTNBTComponent* UHTNTask::GetHTNBTComponent() const
{
	return OwnerBTComponent.Get();
}
