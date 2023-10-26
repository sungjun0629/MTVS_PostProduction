// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionMenuWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Input/SButton.h"
#include "MainMenuWidget.h"
#include "FileToStorageDownloader_Plugin.h"
#include "Framework/Docking/TabManager.h"


void SMotionMenuWidget::Construct(const FArguments& InArgs)
{
	TSharedPtr<SMainMenuWidget> MainMenuWidget;
	//MainMenuWidget->OnGetResponse.BindSP(this, &SMotionMenuWidget::OnGetResponse);
	FVector2D size;
	TSharedPtr<SDockTab> motionTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("Motion Tab"));
	if (motionTab.IsValid())
	{
		size = motionTab->GetDesiredSize();
		UE_LOG(LogTemp,Warning,TEXT("x : %f, y: %f"), size.X, size.Y);
	}


	FString URL = "http://192.168.0.9:8080/view/";

    // Creating the slate widget with an SCanvas and SWebBrowser
	WebBrowserWidget = SNew(SWebBrowser)
		.InitialURL(URL)
		.ShowAddressBar(true)
		.OnUrlChanged_Raw(this, &SMotionMenuWidget::OnURLChanged);

	ChildSlot
	[
		SNew(SCanvas)

			+ SCanvas::Slot()
			.Position(FVector2D(50, 50))
			.Size(FVector2D(1000,500))
//.HAlign(HAlign_Center)
//.VAlign(VAlign_Center)
			[
				WebBrowserWidget.ToSharedRef()
			]

			/*+ SCanvas::Slot()
			.Position(FVector2D(50, 50))
			.Size(FVector2D(100, 100))
			[
				SNew(SButton)
					.OnClicked(this, &SMotionMenuWidget::OnReloadClicked)
					.Text(FText::FromString("Reload"))
			]*/
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

void SMotionMenuWidget::OnGetResponse()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGetResponse"));
}
