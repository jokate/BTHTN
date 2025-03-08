// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/Tasks/SearchTargetTask.h"
#include "HTNModule/HTNBTComponent.h"

bool USearchTargetTask::CheckPrecondition()
{
	UHTNBTComponent* HTNBTComponent = GetHTNBTComponent();

	if ( IsValid(HTNBTComponent) == false )
	{
		return false;
	}

	bool HasTarget = HTNBTComponent->GetSimulatedProperty_Bool("HasTarget");

	return HasTarget == false;
}

void USearchTargetTask::Action()
{
	UE_LOG(LogTemp, Log, TEXT("Finding Target..."));
	Super::Action();
}

//해당 부분은 클래스로 생성된 부분이 있기에 Temp 형태로 시뮬레이팅을 돌리는 게 맞는거로 보임.
//내부 캐싱이 필요함.
// 해당 부분은 Simulate에 대한 특징을 지닌 것에 준한다.
void USearchTargetTask::SimulateEffectToOwner()
{
	UHTNBTComponent* HTNBTComponent = GetHTNBTComponent();

	if ( IsValid(HTNBTComponent) == false )
	{
		return;
	}
	
	HTNBTComponent->AddWorldSimulatedProperty_Bool("HasTarget", true, true);
}

void USearchTargetTask::AfterSimulateEffectToOwner()
{
	UHTNBTComponent* HTNBTComponent = GetHTNBTComponent();

	if ( IsValid(HTNBTComponent) == false )
	{
		return;
	}
	
	HTNBTComponent->AddWorldSimulatedProperty_Bool("HasTarget", true, false);
}
