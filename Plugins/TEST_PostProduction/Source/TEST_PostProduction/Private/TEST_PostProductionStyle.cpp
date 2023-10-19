// Copyright Epic Games, Inc. All Rights Reserved.

#include "TEST_PostProductionStyle.h"
#include "TEST_PostProduction.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FTEST_PostProductionStyle::StyleInstance = nullptr;

void FTEST_PostProductionStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FTEST_PostProductionStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FTEST_PostProductionStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("TEST_PostProductionStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FTEST_PostProductionStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("TEST_PostProductionStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("TEST_PostProduction")->GetBaseDir() / TEXT("Resources"));

	Style->Set("TEST_PostProduction.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FTEST_PostProductionStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FTEST_PostProductionStyle::Get()
{
	return *StyleInstance;
}
