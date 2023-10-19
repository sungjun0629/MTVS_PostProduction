// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_PostProductionCommands.h"

#define LOCTEXT_NAMESPACE "FTEST_PostProductionModule"

void FTEST_PostProductionCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "TEST_PostProduction", "Execute TEST_PostProduction action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
