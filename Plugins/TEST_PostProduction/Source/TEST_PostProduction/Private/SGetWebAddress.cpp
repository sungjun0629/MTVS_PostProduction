// Fill out your copyright notice in the Description page of Project Settings.


#include "SGetWebAddress.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "Widgets/SCanvas.h"
#include "AudioDevice.h"
#include "Engine/Engine.h"
#include "FileToStorageDownloader_Plugin.h"

void SGetWebAddress::Construct(const FArguments& InArgs)
{
	
	FString URL = "https://www.google.com";

	loginWebBrowser = SNew(SWebBrowser)
		.InitialURL(URL)
		//.ShowAddressBar(true)
		.OnUrlChanged_Raw(this , &SGetWebAddress::OnURLChanged);

	ChildSlot
		[
			SNew(SCanvas)

				+ SCanvas::Slot()
				.Position(FVector2D(50 , 50))
				.Size(FVector2D(1000 , 500))
				//.HAlign(HAlign_Center)
				//.VAlign(VAlign_Center)
				[
					loginWebBrowser.ToSharedRef()
				]
		];
}

void SGetWebAddress::OnURLChanged(const FText& InText)
{
	UE_LOG(LogTemp, Warning, TEXT("SGetWebAddress::OnURLChanged : %s"), *InText.ToString());

	if (InText.ToString().Contains("https://storage.cloud.google.com") )
	{
		
	}
	else if ( InText.ToString().Contains("https://accounts.google.com"))
	{

	}
	else
	{
		if(isSound)
		{
			soundConverterLogic->DownloadVoice(InText.ToString(), convertedName);
			isSound = false;
		}else
		{
			UFileToStorageDownloader_Plugin* StorageDownload;
			convertedName = InText.ToString();
			StorageDownload->DownloadFileToStorage(convertedName , storagePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);
		}

		if(loginWebBrowser)
		{
			//loginWebBrowser->~SWebBrowser();
		}
	}
}

void SGetWebAddress::ReloadAndGetURL(FString url, FString ConvertedName)
{
	isSound = true;
	UE_LOG(LogTemp, Warning, TEXT("SGetWebAddress::ReloadAndGetURL : %s"), *url);

	convertedName = ConvertedName;

	FString URL = "https://www.google.com";
	loginWebBrowser = SNew(SWebBrowser)
		.InitialURL(URL)
		//.ShowAddressBar(true)
		.OnUrlChanged_Raw(this , &SGetWebAddress::OnURLChanged);

	if(loginWebBrowser.IsValid())
	loginWebBrowser->LoadURL(url);

}

void SGetWebAddress::ReloadAndGetAssetDownloadURL(FString url, FString StoragePath)
{
	isSound = false;
	storagePath = StoragePath;
	FString URL = url;
	loginWebBrowser = SNew(SWebBrowser)
		.InitialURL(URL)
		//.ShowAddressBar(true)
		.OnUrlChanged_Raw(this , &SGetWebAddress::OnURLChanged);

	if ( loginWebBrowser.IsValid() )
	{
		//loginWebBrowser->LoadURL(url);
		UE_LOG(LogTemp,Warning,TEXT("ReloadAndGetAssetDownloadURL : %s"), *url)
	}
}
