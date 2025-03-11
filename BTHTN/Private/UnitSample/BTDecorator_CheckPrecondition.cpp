// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/BTDecorator_CheckPrecondition.h"
#include "AIController.h"
#include "HTNModule/HTNBTComponent.h"
#include "HTNModule/Interface/HTNBTPlanner.h"

bool UBTDecorator_CheckPrecondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if ( IsValid(AIController) == false)
	{
		return false;	
	}

	IHTNBTPlanner* Planner = Cast<IHTNBTPlanner>(AIController);

	if ( Planner == nullptr )
	{
		return false;
	}

	UHTNBTComponent* PlanningComponent = Planner->GetHTNBTComponent();

	if ( IsValid(PlanningComponent) == false)
	{
		return false;
	}

	UHTNTask* FirstTask = PlanningComponent->GetFirstTaskInPlan();

	if ( IsValid(FirstTask) == false )
	{
		return false;
	}

	return FirstTask->CheckPrecondition();
}

void UBTDecorator_CheckPrecondition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionResultChanged);
}
