// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "HTNTask.h"
#include "HTNTaskConditionalValue.h"
#include "Components/ActorComponent.h"
#include "Interface/HTNBTPlanner.h"
#include "HTNBTComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BTHTN_API UHTNBTComponent : public UActorComponent, public IHTNBTPlanner
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHTNBTComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Task Planning
	// Plan Task.
	virtual void SimulatePlanningTask();

	virtual UHTNTask* GetTaskByTag( FGameplayTag& TaskTag );

#pragma endregion Task Planning

#pragma region IHTNBTPlanner
	// IHTNBTPlanner method declaration
	virtual void OnTaskFinished( FGameplayTag& FinishedTaskTag ) override;

	virtual FGameplayTag GetTaskTagToActive() override;

	// If something was wrong by external factor.
	virtual void ResetAllTaskTag() override
	{
		TaskTagsToActive.Empty();
	};

	// We need to check running plan because of external factor.
	virtual UHTNTask* GetFirstTaskInPlan() override;

	//Need To Update World State Integer Value.
	UFUNCTION()
	virtual void UpdateWorldIntegerValue( FName KeyName, int32 UpdatedValue) override {}

	UFUNCTION()
	virtual void UpdateWorldBooleanValue( FName KeyName, bool UpdatedValue) override {  }

	UFUNCTION()
	virtual void UpdateWorldFloatValue( FName KeyName, float UpdatedValue ) override { }
#pragma endregion IHTNBTPlanner
	
	UPROPERTY(VisibleAnywhere, Category = "HTN Task | Task To Active")
	TArray<FGameplayTag> TaskTagsToActive;

	UPROPERTY(EditDefaultsOnly, Category = "HTN Task | Tag To Allocate")
	TArray<UHTNTask*> RegisteredTask;
};
