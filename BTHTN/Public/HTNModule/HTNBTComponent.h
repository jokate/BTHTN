// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HTNTask.h"
#include "HTNTaskConditionalValue.h"
#include "Components/ActorComponent.h"
#include "Interface/HTNBTPlanner.h"
#include "HTNBTComponent.generated.h"

USTRUCT()
struct FSequenceQueryStruct
{
	GENERATED_BODY()

public :
	UPROPERTY()
	FGameplayTag OwnerTaskTag;

	UPROPERTY()
	FGameplayTag TaskTag;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BTHTN_API UHTNBTComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHTNBTComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void RegisterTask( TSubclassOf<UHTNTask> HTNTaskClass );

#pragma region Task Planning
	// Plan Task.
	virtual void SimulatePlanningTask();

	virtual UHTNTask* GetTaskByTag( FGameplayTag& TaskTag );

#pragma endregion Task Planning
	
	virtual void OnTaskFinished( FGameplayTag& FinishedTaskTag );

	virtual FGameplayTag GetTaskTagToActive();

	// If something was wrong by external factor.
	virtual void ResetAllTaskTag()
	{
		TaskTagsToActive.Empty();
	};

	// We need to check running plan because of external factor.
	virtual UHTNTask* GetFirstTaskInPlan();

	virtual void RemoveFirstTagInTaskList();

#pragma region Task World State
	
	virtual void AddTaskWorldState(UTaskWorldState* WorldState);

	virtual void AddTaskWorldStateByClass(TSubclassOf<UTaskWorldState> WorldStateClass);
	
	UTaskWorldState* GetTaskWorldState() const { return SpawnedTaskWorldState; }
	
#pragma endregion Task World State
	
protected :
	UPROPERTY(VisibleAnywhere, Category = "HTN Task | Task To Active")
	TArray<FGameplayTag> TaskTagsToActive;
	
	UPROPERTY(VisibleAnywhere, Category = "HTN Task | Tag To Allocate")
	TMap<FGameplayTag, UHTNTask*> RegisteredTask;
	
	UPROPERTY( VisibleAnywhere, Category = "HTN Task | Spawned Task")
	TObjectPtr<UTaskWorldState> SpawnedTaskWorldState;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag RootGameplayTag;
};

