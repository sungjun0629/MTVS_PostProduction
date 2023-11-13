// Fill out your copyright notice in the Description page of Project Settings.


#include "SGetWebAddress.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "Widgets/SCanvas.h"
#include "AudioDevice.h"
#include "Engine/Engine.h"

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

	if (!InText.ToString().Contains("https://storage.cloud.google.com") )
	{
		soundConverterLogic->DownloadVoice(InText.ToString());
		loginWebBrowser->~SWebBrowser();
	}
}

void SGetWebAddress::ReloadAndGetURL(FString url)
{
	UE_LOG(LogTemp, Warning, TEXT("SGetWebAddress::ReloadAndGetURL"),);

	FString URL = "https://www.google.com";
	loginWebBrowser = SNew(SWebBrowser)
		.InitialURL(URL)
		//.ShowAddressBar(true)
		.OnUrlChanged_Raw(this , &SGetWebAddress::OnURLChanged);

	if(loginWebBrowser.IsValid())
	loginWebBrowser->LoadURL(url);

}
