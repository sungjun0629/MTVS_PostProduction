// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "SoundConverterLogic.h"
#include "WebBrowser/Public/SWebBrowser.h"
/**
 * 
 */
class SGetWebAddress : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SGetWebAddress) {}

	SLATE_END_ARGS()


public:

	FOnDownloadProgress OnDownloadProgressDelegate;

	FOnFileToStorageDownloadComplete OnFileToStorageDownloadCompleteDelegate;

	class USoundConverterLogic* soundConverterLogic = NewObject<USoundConverterLogic>();

	FString convertedURL;

	void Construct(const FArguments& InArgs);

	TSharedPtr<SWebBrowser> loginWebBrowser;

	void OnURLChanged(const FText& InText);

	void ReloadAndGetURL(FString url, FString ConvertedName);
	void ReloadAndGetAssetDownloadURL(FString url, FString StoragePath);

	FString convertedName;
	FString storagePath;
	bool isSound = false;
};
