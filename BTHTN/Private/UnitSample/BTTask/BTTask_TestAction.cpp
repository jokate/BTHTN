// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/BTTask/BTTask_TestAction.h"

#include "AIController.h"
#include "HTNModule/HTNBTComponent.h"
#include "HTNModule/Interface/HTNBTPlanner.h"

EBTNodeResult::Type UBTTask_TestAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();

	if ( IsValid( Controller ) == false)
	{
		return EBTNodeResult::Failed;
	}
	
	IHTNBTPlanner* Planner = Cast<IHTNBTPlanner>(Controller);

	if ( Planner == nullptr )
	{
		return EBTNodeResult::Failed;
	}

	UHTNBTComponent* PlanningComponent = Planner->GetHTNBTComponent();

	if ( IsValid( PlanningComponent ) == false)
	{
		return EBTNodeResult::Failed;
	}

	UHTNTask* FirstExecutionTask = PlanningComponent->GetFirstTaskInPlan();

	if ( IsValid(FirstExecutionTask) == false )
	{	
		return EBTNodeResult::Failed;	
	}

	FirstExecutionTask->TestActionLog();

	PlanningComponent->RemoveFirstTagInTaskList();
	
	return EBTNodeResult::Succeeded;
}

