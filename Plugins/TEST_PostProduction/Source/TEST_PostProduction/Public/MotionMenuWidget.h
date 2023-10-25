// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "WebBrowser/Public/SWebBrowserView.h"


class SMotionMenuWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMotionMenuWidget) {}

	SLATE_END_ARGS()

public:

	TSharedPtr<SWebBrowser> WebBrowserWidget;

	void Construct(const FArguments& InArgs);

	FReply OnReloadClicked();

	void OnURLChanged(const FText& InText);

	FOnDownloadProgress OnDownloadProgressDelegate;

	FOnFileToStorageDownloadComplete OnFileToStorageDownloadCompleteDelegate;

	FOnGetResponse OnGetResponseDelegate;

	UFUNCTION()
	void OnGetResponse();
};