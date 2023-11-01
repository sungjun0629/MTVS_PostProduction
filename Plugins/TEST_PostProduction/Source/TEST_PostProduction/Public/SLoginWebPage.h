// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "WebBrowser/Public/SWebBrowserView.h"
/**
 * 
 */
class SLoginWebPage : public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SLoginWebPage) {}

	SLATE_END_ARGS()

public:
	bool DoOnceBool = false;

	void Construct(const FArguments& InArgs);

	TSharedPtr<SWebBrowser> loginWebBrowser;

	void OnURLChanged(const FText& InText);
};
