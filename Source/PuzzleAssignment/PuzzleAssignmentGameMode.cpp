// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzleAssignmentGameMode.h"
#include "PuzzleAssignmentCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuzzleAssignmentGameMode::APuzzleAssignmentGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
