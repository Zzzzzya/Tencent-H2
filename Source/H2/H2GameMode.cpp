// Copyright Epic Games, Inc. All Rights Reserved.

#include "H2GameMode.h"
#include "H2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GameStateShootScore.h"

AH2GameMode::AH2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	GameStateClass = AGameStateShootScore::StaticClass();

}

void AH2GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}
