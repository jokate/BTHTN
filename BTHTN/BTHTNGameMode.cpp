// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTHTNGameMode.h"
#include "BTHTNCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABTHTNGameMode::ABTHTNGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
