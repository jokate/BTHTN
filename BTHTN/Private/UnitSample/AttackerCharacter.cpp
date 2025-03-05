// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitSample/AttackerCharacter.h"

#include "HTNModule/HTNBTComponent.h"
#include "UnitSample/HTNBTAIController.h"


// Sets default values
AAttackerCharacter::AAttackerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAttackerCharacter::BeginPlay()
{
	Super::BeginPlay();
	RegisterTask();
}

void AAttackerCharacter::RegisterTask()
{
	UHTNBTComponent* BTComponent = GetHTNBTComponent();
	if ( IsValid(BTComponent) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("HTN BT Component Is Not Valid : Register Failed"));
	}

	for ( TSubclassOf<UHTNTask>& HTNTask : HTNTaskClass)
	{
		BTComponent->RegisterTask(HTNTask);
	}
}

// Called every frame
void AAttackerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAttackerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UHTNBTComponent* AAttackerCharacter::GetHTNBTComponent()
{
	IHTNBTPlanner* Planner = GetController<IHTNBTPlanner>();

	if ( Planner == nullptr )
	{
		return nullptr;
	}

	return Planner->GetHTNBTComponent();
}

