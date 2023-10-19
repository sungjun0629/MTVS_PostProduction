// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TEST_PostProductionStyle.h"

class FTEST_PostProductionCommands : public TCommands<FTEST_PostProductionCommands>
{
public:

	FTEST_PostProductionCommands()
		: TCommands<FTEST_PostProductionCommands>(TEXT("TEST_PostProduction"), NSLOCTEXT("Contexts", "TEST_PostProduction", "TEST_PostProduction Plugin"), NAME_None, FTEST_PostProductionStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
