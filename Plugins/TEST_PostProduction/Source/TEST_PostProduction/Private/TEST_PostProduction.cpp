// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_PostProduction.h"
#include "TEST_PostProductionStyle.h"
#include "TEST_PostProductionCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "ContentBrowserModule.h"
#include "Modules/ModuleManager.h"
#include "PostProductionWidget.h"
#include "MainMenuWidget.h"
#include "MotionMenuWidget.h"
#include "SoundConvertWidget.h"
#include "SLoginWebPage.h"
#include "SequencerPractice.h"

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

	
	RegisterCustomEditorTab();
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
	FGlobalTabmanager::Get()->TryInvokeTab(FName("PostProduction"));
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


#pragma region CustomEditorTab

void FTEST_PostProductionModule::RegisterCustomEditorTab()
{
	// Enroll NomadTab to GlobalTabmanager
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("PostProduction"), FOnSpawnTab::CreateRaw(this, &FTEST_PostProductionModule::OnSpawnPostProductionTab)).SetDisplayName(FText::FromString("PostProduiction Tab"));

	// Enroll Video Tab to GlobalTabmanager
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("Video Tab"), FOnSpawnTab::CreateRaw(this, &FTEST_PostProductionModule::OnSpawnMainMenuTab)).SetDisplayName(FText::FromString("Video Tab"));
	
	// Enroll MotionTab to GlobalTabmanager
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("Motion Tab"), FOnSpawnTab::CreateRaw(this, &FTEST_PostProductionModule::OnSpawnWebTab)).SetDisplayName(FText::FromString("Motion Tab"));

	// Enroll SoundTab to GlobalTabmanager
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("Sound Tab"), FOnSpawnTab::CreateRaw(this, &FTEST_PostProductionModule::OnSpawnSoundTab)).SetDisplayName(FText::FromString("Sound Tab"));

	// Enroll SoundTab to GlobalTabmanager
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("Login Tab"), FOnSpawnTab::CreateRaw(this, &FTEST_PostProductionModule::OnSpawnLoginTab)).SetDisplayName(FText::FromString("Login Tab"));

	// Enroll memoTab to GlobalTabmanager
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("Memo Tab"), FOnSpawnTab::CreateRaw(this, &FTEST_PostProductionModule::OnSpawnMemoTab)).SetDisplayName(FText::FromString("Memo Tab"));
}

TSharedRef<SDockTab> FTEST_PostProductionModule::OnSpawnPostProductionTab(const FSpawnTabArgs& spawnArgs)
{
	// NomadTab : can be dragged out
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SPostProductionWidget)
	]; 
}

TSharedRef<SDockTab> FTEST_PostProductionModule::OnSpawnMainMenuTab(const FSpawnTabArgs& spawnArgs)
{
	// NomadTab : can be dragged out
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SMainMenuWidget)
		];
}

TSharedRef<SDockTab> FTEST_PostProductionModule::OnSpawnWebTab(const FSpawnTabArgs& spawnArgs)
{
	// NomadTab : can be dragged out
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SMotionMenuWidget)
		];
}

TSharedRef<SDockTab> FTEST_PostProductionModule::OnSpawnSoundTab(const FSpawnTabArgs& spawnArgs)
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SSoundConvertWidget)
		];
}

TSharedRef<SDockTab> FTEST_PostProductionModule::OnSpawnLoginTab(const FSpawnTabArgs& spawnArgs)
{
	// NomadTab : can be dragged out
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SLoginWebPage)
		];
}

TSharedRef<SDockTab> FTEST_PostProductionModule::OnSpawnMemoTab(const FSpawnTabArgs& spawnArgs)
{
	// NomadTab : can be dragged out
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SSequencePractice)
		];
}

#pragma endregion 

#pragma region ContentBrowserMenuExtention

void FTEST_PostProductionModule::InitCBMenuExtension()
{
	//<---------- Added a Custom Delegate to menu extender ---------->
	// Load Content Browser Module
	FContentBrowserModule& ContentBrowserModule = 
		FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("Content Browser"));

	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders =
		ContentBrowserModule.GetAllPathViewContextMenuExtenders();
	
	// custom Delegate, add Address to enroll the function
	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this, &FTEST_PostProductionModule::CustomCBMenuExtender);
	ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate);

	// Add and Bind in One Line
	//ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this,&FTEST_PostProductionModule::CustomCBMenuExtender));



}

TSharedRef<FExtender> FTEST_PostProductionModule::CustomCBMenuExtender(const TArray<FString>& SeletedPaths)
{
	TSharedRef<FExtender> MenuExtender (new FExtender());

	// how many folders are selected
	if (SeletedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"),
		EExtensionHook::After, 
		TSharedPtr<FUICommandList>(),
		// second Binding : Define the details for the menu entry
		FMenuExtensionDelegate::CreateRaw(this, &FTEST_PostProductionModule::AddCBMenuEntry)
		);
	}

	return MenuExtender;
}

void FTEST_PostProductionModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry
	(
		LOCTEXT("TEST_PostProduction", "TEST_PostProduction"),
		LOCTEXT("TEST_PostProduction_ToolTip", "TEST_PostProduction"),
		FSlateIcon(),
		//third Binding : the actual function to execute
		FUIAction(FExecuteAction::CreateRaw(this, &FTEST_PostProductionModule::OnDeleteUnusedAssetButtonClicked))
	);
}

void FTEST_PostProductionModule::OnDeleteUnusedAssetButtonClicked()
{
	FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("TEST_PostProduction", "TEST_PostProduction"));
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTEST_PostProductionModule, TEST_PostProduction)