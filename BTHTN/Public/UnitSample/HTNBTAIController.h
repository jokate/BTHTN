// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HTNModule/Interface/HTNBTPlanner.h"
#include "HTNBTAIController.generated.h"

UCLASS()
class BTHTN_API AHTNBTAIController : public AAIController, public IHTNBTPlanner
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHTNBTAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void RunAI();

	virtual void OnPossess(APawn* InPawn) override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UHTNBTComponent* GetHTNBTComponent() override;

protected :

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	TObjectPtr<class UHTNBTComponent> HTNBTComponent;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	TSubclassOf<class UTaskWorldState> AttackerWorldStateClass;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBlackboardData> BBAsset;
};
