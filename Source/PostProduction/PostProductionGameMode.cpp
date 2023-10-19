// Copyright Epic Games, Inc. All Rights Reserved.

#include "PostProductionGameMode.h"
#include "PostProductionCharacter.h"
#include "UObject/ConstructorHelpers.h"

APostProductionGameMode::APostProductionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
