// Fill out your copyright notice in the Description page of Project Settings.


#include "SLoginWebPage.h"
#include "IPConfig.h"
#include "LoginLogic_Plugin.h"
#include "Kismet/KismetStringLibrary.h"
#include "Widgets/SCanvas.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include "TimerManager.h"
#include "Containers/Ticker.h"
#include "Framework/Docking/TabManager.h"



void SLoginWebPage::Construct(const FArguments& InArgs)
{
	FString URL = IPConfig::StaticVariable + "/oauth2/authorize/kakao";

	loginWebBrowser = SNew(SWebBrowser)
		.InitialURL(URL)
		.ShowAddressBar(true)
		.OnUrlChanged_Raw(this, &SLoginWebPage::OnURLChanged);

	ChildSlot
		[
			SNew(SCanvas)

				+ SCanvas::Slot()
				.Position(FVector2D(50, 50))
				.Size(FVector2D(1000, 500))
				//.HAlign(HAlign_Center)
				//.VAlign(VAlign_Center)
				[
					loginWebBrowser.ToSharedRef()
				]
		];

}

void SLoginWebPage::OnURLChanged(const FText& InText)
{
	UE_LOG(LogTemp, Warning, TEXT("SLoginWebPage::OnURLChanged"),);
	URLString = InText.ToString();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *URLString);

	static FString CheckPointString = "https://logins.daum.net/accounts/kakaossotokenlogin.do?redirect=true&ssotoken=";
	static FString NextStepString = IPConfig::StaticVariable;

	bool NeedValueString = UKismetStringLibrary::Contains(URLString, NextStepString);

	if (NeedValueString && DoOnceBool == false)
	{
		OnGetToken();
	}

}


void SLoginWebPage::OnGetToken()
{

	auto Callback = [=](const FString& SourceURL) {
		// Handle the source URL as needed
		ParsingHtml(SourceURL);
		UE_LOG(LogTemp, Warning, TEXT("SourceURL : %s"), *SourceURL);
		};

	loginWebBrowser->GetSource(Callback);

}

void SLoginWebPage::ParsingHtml(FString HtmlString)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Before Parseing : %s"), *HtmlString);

	HtmlString.Split(TEXT("{"), nullptr, &HtmlString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	HtmlString.Split(TEXT(":"), nullptr, &HtmlString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	HtmlString.Split(TEXT("\""), nullptr, &HtmlString, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	HtmlString.Split(TEXT("\""), &HtmlString, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromStart);

	UE_LOG(LogTemp, Warning, TEXT("Access Token : %s"), *HtmlString);
	IPConfig::Token = HtmlString;

	// 화면 전환
	ConvertTab();
}

void SLoginWebPage::ConvertTab()
{	
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Video Tab"));
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Sound Tab"));
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Motion Tab"));

	// Close the old tab if it exists
	TSharedPtr<SDockTab> OldTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("Login Tab"));
	if (OldTab.IsValid())
	{
		OldTab->RequestCloseTab();
	}
}

void SLoginWebPage::SequencerTest()
{
	//FMovieSceneSequenceEditor* SequenceEditor;
	//if (SequenceEditor)
	//{
	//	UMovieSceneSequence* Sequence = SequenceEditor->GetFocusedMovieSceneSequence();
	//	if (Sequence)
	//	{
	//		FGuid ObjectGuid; // Fill in the appropriate object GUID for the object you want to add the keyframe to
	//		FFrameNumber CurrentTime = SequenceEditor->GetLocalTime().Time;
	//		FMovieSceneObjectBindingID ObjectBindingID(ObjectGuid, EMovieSceneObjectBindingSpace::Local);

	//		FScopedTransaction Transaction(FText::FromString("Add Transform Keyframe in Sequencer"));
	//		Sequence->Modify();

	//		UMovieScene* MovieScene = Sequence->GetMovieScene();
	//		if (MovieScene)
	//		{
	//			UMovieScene3DTransformSection* TransformSection = Cast<UMovieScene3DTransformSection>(MovieScene->FindOrCreateSection(ObjectBindingID, UMovieScene3DTransformSection::StaticClass(), CurrentTime));
	//			if (TransformSection)
	//			{
	//				FMovieScene3DTransformKey NewKey;
	//				NewKey.Translation = Location;
	//				NewKey.Rotation = FRotator::ZeroRotator; // Set the rotation as needed
	//				NewKey.Scale = FVector::OneVector; // Set the scale as needed
	//				TransformSection->AddKey(CurrentTime, NewKey);
	//			}
	//		}
	//	}
	//}

}
