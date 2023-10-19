// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_PostProduction.h"
#include "TEST_PostProductionStyle.h"
#include "TEST_PostProductionCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName TEST_PostProductionTabName("TEST_PostProduction");

#define LOCTEXT_NAMESPACE "FTEST_PostProductionModule"

void FTEST_PostProductionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTEST_PostProductionStyle::Initialize();
	FTEST_PostProductionStyle::ReloadTextures();

	FTEST_PostProductionCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTEST_PostProductionCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FTEST_PostProductionModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTEST_PostProductionModule::RegisterMenus));
}

void FTEST_PostProductionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FTEST_PostProductionStyle::Shutdown();

	FTEST_PostProductionCommands::Unregister();
}

void FTEST_PostProductionModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FTEST_PostProductionModule::PluginButtonClicked()")),
							FText::FromString(TEXT("TEST_PostProduction.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FTEST_PostProductionModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FTEST_PostProductionCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FTEST_PostProductionCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTEST_PostProductionModule, TEST_PostProduction)