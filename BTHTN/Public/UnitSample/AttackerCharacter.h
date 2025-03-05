// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HTNModule/Interface/HTNBTPlanner.h"
#include "AttackerCharacter.generated.h"

UCLASS()
class BTHTN_API AAttackerCharacter : public ACharacter, public IHTNBTPlanner
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAttackerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RegisterTask();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UHTNBTComponent* GetHTNBTComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly )
	TArray<TSubclassOf<class UHTNTask>> HTNTaskClass;
};
