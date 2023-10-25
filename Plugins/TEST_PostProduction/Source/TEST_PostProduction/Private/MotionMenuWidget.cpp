// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMenuWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Input/SButton.h"
#include "FileToStorageDownloader_Plugin.h"

void SMotionMenuWidget::Construct(const FArguments& InArgs)
{
	FString URL = "http://192.168.0.4:8080/view";

    // Creating the slate widget with an SCanvas and SWebBrowser
	WebBrowserWidget = SNew(SWebBrowser)
		.InitialURL(URL)
		.ViewportSize(FVector2D(500,500))
		.ShowAddressBar(true)
		.OnUrlChanged_Raw(this, &SMotionMenuWidget::OnURLChanged);

	ChildSlot
	[
		SNew(SCanvas)

			+ SCanvas::Slot()
			.Position(FVector2D(50, 50))
			.Size(FVector2D(700, 500))
			[
				WebBrowserWidget.ToSharedRef()
			]

			+ SCanvas::Slot()
			.Position(FVector2D(50, 50))
			.Size(FVector2D(100, 100))
			[
				SNew(SButton)
					.OnClicked(this, &SMotionMenuWidget::OnReloadClicked)
					.Text(FText::FromString("Reload"))
			]
	];
}

FReply SMotionMenuWidget::OnReloadClicked()
{
    WebBrowserWidget->LoadURL("http://192.168.0.4:8080/view/");
	UE_LOG(LogTemp, Warning, TEXT("OnReloadClicked : %s") , *WebBrowserWidget->GetUrl());


	return FReply::Handled();
}

void SMotionMenuWidget::OnURLChanged(const FText& InText)
{
	UE_LOG(LogTemp, Warning, TEXT("OnURLChanged : %s"), *InText.ToString());

	FString FBXURL="";
	FString SavePath = "D:\\DownTest\\File2.fbx";
	UFileToStorageDownloader_Plugin* StorageDownload;

	if (InText.ToString().Contains("result?fbxUrl="))
	{
	    InText.ToString().Split("=", nullptr, &FBXURL);
		if(FBXURL.Len() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("DownloadStart : %s"), *FBXURL);
			StorageDownload->DownloadFileToStorage(FBXURL, SavePath , 15.f, "", true, OnDownloadProgressDelegate, OnFileToStorageDownloadCompleteDelegate);
		}
	}
}
