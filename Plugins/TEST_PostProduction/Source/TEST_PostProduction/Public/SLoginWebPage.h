// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "WebBrowser/Public/SWebBrowserView.h"
#include "Engine/TimerHandle.h"
/**
 * 
 */
class SLoginWebPage : public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SLoginWebPage) {}

	SLATE_END_ARGS()

public:

	FString rawHtml;

	FString URLString;

	FDelegateHandle tikcerHandle;

	bool DoOnceBool = false;

	void Construct(const FArguments& InArgs);

	TSharedPtr<SWebBrowser> loginWebBrowser;

	void OnURLChanged(const FText& InText);

	bool OnTicker(float DeltaTime);

	void ParsingHtml(FString HtmlString);

	void ConvertTab();
};
