// Fill out your copyright notice in the Description page of Project Settings.


#include "SLoginWebPage.h"
#include "IPConfig.h"
#include "LoginLogic_Plugin.h"
#include "Kismet/KismetStringLibrary.h"
#include "Widgets/SCanvas.h"

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
	LoginLogic_Plugin* loginLogic = new LoginLogic_Plugin();

	FString URLString = InText.ToString();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *URLString);

	static FString CheckPointString = "https://logins.daum.net/accounts/kakaossotokenlogin.do?redirect=true&ssotoken=";
	static FString NextStepString = "http://192.168.1.11:8080/oauth2/callback/kakao?code=";

	//bool NeedValueString = UKismetStringLibrary::Contains(URLString, NextStepString);
	bool NeedValueString = UKismetStringLibrary::Contains(URLString, NextStepString);

	if (NeedValueString && DoOnceBool == false)
	{
		DoOnceBool = true;
		loginLogic->GetHeaderToken(URLString);
	}
}
