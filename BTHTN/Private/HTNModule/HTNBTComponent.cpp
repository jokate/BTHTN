// Fill out your copyright notice in the Description page of Project Settings.


#include "HTNModule/HTNBTComponent.h"

#include "AIController.h"
#include "HTNModule/HTNTask.h"
#include "HTNModule/TaskWorldState.h"
#include "UnitSample/AttackerWorldState.h"


// Sets default values for this component's properties
UHTNBTComponent::UHTNBTComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHTNBTComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHTNBTComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for ( TPair<FGameplayTag, UHTNTask*> HTNTask : RegisteredTask )
	{
		UHTNTask* Task = HTNTask.Value;

		if ( IsValid(Task) == true)
		{
			Task->ConditionalBeginDestroy();
			Task->MarkAsGarbage();
		}
	}

	RegisteredTask.Empty();

	
	SpawnedTaskWorldState->ConditionalBeginDestroy();
	
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UHTNBTComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Task need to be planned instantly when task tag container is empty.
	SimulatePlanningTask();
}

void UHTNBTComponent::RegisterTask(TSubclassOf<UHTNTask> HTNTaskClass )
{
	AActor* Owner = GetOwner();

	if ( IsValid(Owner) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UHTNBTComponent::RegisterTask : Owner Is Not Valid"));
		return;
	}

	UHTNTask* HTNTask = NewObject<UHTNTask>(Owner, HTNTaskClass);

	if ( IsValid(HTNTask) == false )
	{
		UE_LOG(LogTemp, Error, TEXT("UHTNBTComponent::RegisterTask : Task Creation Failed"));
		return;
	}
	
	if ( RegisteredTask.Contains(HTNTask->GetTaskTag()) == true )
	{
		UE_LOG(LogTemp, Error, TEXT("UHTNBTComponent::RegisterTask : Task Tag Is Alreay Defined"));
		HTNTask->BeginDestroy();
		return;
	}
	HTNTask->InitializeHTNComponent(this);
	RegisteredTask.Add(HTNTask->GetTaskTag(), HTNTask);
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
	TArray<FGameplayTag> FinalPlans;
	
	TempTaskGameplayTag.Add(RootGameplayTag);
	while (TempTaskGameplayTag.IsEmpty() == false)
	{
		FGameplayTag TaskGameplayTag = TempTaskGameplayTag.Pop();

		UHTNTask* HTNTask = GetTaskByTag(TaskGameplayTag);

		if ( IsValid(HTNTask) == false )
		{
			continue;
		}

		if ( HTNTask->CheckPrecondition() == false )
		{
			continue;
		}

		EHTNTaskType HTNTaskType = HTNTask->GetTaskType();

		// Compound Task Case.
		if ( HTNTaskType == EHTNTaskType::COMPOUND )
		{
			// Find Precondition Match.
			TArray<FGameplayTag> PossibleTagArray;
			FGameplayTagContainer& PossibleTag = HTNTask->GetPossibleNextTag();
			PossibleTag.GetGameplayTagArray(PossibleTagArray);

			UHTNTask* ResultTask = nullptr;
			for ( FGameplayTag& TaskTag : PossibleTagArray )
			{
				// Check Task Precondition	
				UHTNTask* PossibleTask = GetTaskByTag( TaskTag );

				if ( IsValid( PossibleTask ) == false )
				{
					continue;
				}

				if ( PossibleTask->CheckPrecondition() == true )
				{
					ResultTask = PossibleTask;
					break;
				}
			}

			// if Found Precondition Match Task.
			if ( IsValid(ResultTask) == true )
			{
				// Sequence 저장. ( Record하는 방법은 분명 유효함. -> 단 내부에서 빠져주는 부분이 있어야 함. )
				// 어느정도 유연하게 교체가 되어야 한다.
				// 시퀀스 Tag들을 싹다 넣어줘야 함.
				TArray<FGameplayTag> SubsequenceArray;
				ResultTask->GetSequenceTagContainer().GetGameplayTagArray(SubsequenceArray);

				for ( int32 SequenceTagLength = SubsequenceArray.Num() - 1; SubsequenceArray.Num() > 0; SequenceTagLength-- )
				{
					TempTaskGameplayTag.Add(SubsequenceArray[SequenceTagLength]);
				}
			}
			else
			{
				//Search Failed.
				for (FGameplayTag& GameplayTag : FinalPlans)
				{
					UHTNTask* Task = GetTaskByTag(GameplayTag);

					if ( IsValid(Task) == true )
					{
						Task->SimulateEffectToOwner( false );
					} 
				}

				FinalPlans.Empty();
				
			}
		}
		else
		{
			// if Primitive Task Simply Add.
			if ( HTNTask->CheckPrecondition() == true )
			{
				HTNTask->SimulateEffectToOwner( true );
				FinalPlans.Add( HTNTask->GetTaskTag() );
			}
			else
			{
				//Search Failed.
				for (FGameplayTag& GameplayTag : FinalPlans)
				{
					UHTNTask* Task = GetTaskByTag(GameplayTag);

					if ( IsValid(Task) == true )
					{
						Task->SimulateEffectToOwner( false );
					} 
				}

				FinalPlans.Empty();
				break;
			}
		}
	}
	
	TaskTagsToActive = FinalPlans;
}


UHTNTask* UHTNBTComponent::GetTaskByTag(FGameplayTag& TaskTag)
{
	for ( TPair<FGameplayTag, UHTNTask*> Task : RegisteredTask )
	{
		if ( Task.Key == TaskTag )
		{
			return Task.Value;
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

void UHTNBTComponent::RemoveFirstTagInTaskList()
{
	if ( TaskTagsToActive.IsValidIndex(0) == true )
	{
		TaskTagsToActive.RemoveAt(0);
	}
}


void UHTNBTComponent::AddTaskWorldState(UTaskWorldState* WorldState)
{
	if ( IsValid(WorldState) == true )
	{
		WorldState->SetupStructProperties();
		SpawnedTaskWorldState  = WorldState;
	}
}

void UHTNBTComponent::AddTaskWorldStateByClass(TSubclassOf<UTaskWorldState> WorldStateClass)
{
	AAIController* Controller = Cast<AAIController>(GetOwner());
	UTaskWorldState* WorldState = NewObject<UTaskWorldState>( Controller->GetPawn(), WorldStateClass );

	if ( IsValid(WorldState) == true )
	{
		WorldState->SetupStructProperties();
		SpawnedTaskWorldState = WorldState;
	}
}
