// Fill out your copyright notice in the Description page of Project Settings.


#include "SLoginWebPage.h"
#include "IPConfig.h"
#include "LoginLogic_Plugin.h"
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

	loginLogic->GetHeaderToken(InText.ToString());
}
