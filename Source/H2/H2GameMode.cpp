// Copyright Epic Games, Inc. All Rights Reserved.

#include "H2GameMode.h"
#include "H2Character.h"
#include "UObject/ConstructorHelpers.h"

AH2GameMode::AH2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
