// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPS_BoxShooterGameMode.h"
#include "FPS_BoxShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPS_BoxShooterGameMode::AFPS_BoxShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
