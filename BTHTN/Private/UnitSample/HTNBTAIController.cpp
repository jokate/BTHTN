// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/HTNBTAIController.h"
#include "HTNModule/HTNBTComponent.h"
#include "UnitSample/AttackerWorldState.h"


// Sets default values
AHTNBTAIController::AHTNBTAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HTNBTComponent = CreateDefaultSubobject<UHTNBTComponent>("HTNBTComponent");
}

// Called when the game starts or when spawned
void AHTNBTAIController::BeginPlay()
{
	Super::BeginPlay();

	if ( IsValid(HTNBTComponent) == true )
	{
		HTNBTComponent->AddTaskWorldStateByClass(AttackerWorldStateClass);
	}
}

void AHTNBTAIController::RunAI()
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();

	bool bUseBlackBoard = UseBlackboard(BBAsset, BlackboardComp);

	if (bUseBlackBoard)
	{
		RunBehaviorTree(BTAsset);
	}
}

void AHTNBTAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

// Called every frame
void AHTNBTAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UHTNBTComponent* AHTNBTAIController::GetHTNBTComponent()
{
	if ( IsValid(HTNBTComponent) == false )
	{
		return nullptr;
	}

	return HTNBTComponent;
}

