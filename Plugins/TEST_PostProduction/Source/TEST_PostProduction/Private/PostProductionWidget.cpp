// Fill out your copyright notice in the Description page of Project Settings.


#include "PostProductionWidget.h"

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/STileView.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "Framework/Docking/TabManager.h"


void SPostProductionWidget::Construct(const FArguments& InArgs)
{
	// Set the content scale for this widget.
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	FSlateFontInfo SubTitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 35;
	SubTitleTextFont.Size = 15;


	ChildSlot
		[	//Main vertical box
			SNew(SVerticalBox)

				//First vertical slot for title text
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0,40,0,0)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("LIFE")))
						.Font(TitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 20, 0, 80)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Give Life to Digital Character")))
						.Font(SubTitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]

				// Second vertical slot for username and password
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(100,0, 100,30)
				[
					SNew(SHorizontalBox)
					
					// Kakao Login Layout
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(50, 0, 20, 0)
					[
						SNew(SButton)
							.OnClicked(this, &SPostProductionWidget::OnKakaoLoginClicked)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
									.Text(FText::FromString("Kakao Login"))
									.ColorAndOpacity(FSlateColor(FLinearColor::White))
							]
					]


					// GitHub Login Layout
					+SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0, 0, 0, 0)
					[
						SNew(SButton)
							.VAlign(VAlign_Center)
							.Text(FText::FromString("GitHub Login"))
							.OnClicked(this, &SPostProductionWidget::OnGitHubLoginClicked)
							.HAlign(HAlign_Center)
					]
				]
		];
}

FReply SPostProductionWidget::OnKakaoLoginClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Kakao Login Clicked"));

	// Open the new editor tab
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Video Tab"));
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Motion Tab"));

	// Close the old tab if it exists
	TSharedPtr<SDockTab> OldTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("PostProduction"));
	if (OldTab.IsValid())
	{
		OldTab->RequestCloseTab();
	}

	return FReply::Handled();
}

FReply SPostProductionWidget::OnGitHubLoginClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("GitHub Login Clicked"));

	// Open the new editor tab
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Video Tab"));
	FGlobalTabmanager::Get()->TryInvokeTab(FName("Motion Tab"));
	
	// Close the old tab if it exists
	TSharedPtr<SDockTab> OldTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("PostProduction"));
	if (OldTab.IsValid())
	{
		OldTab->RequestCloseTab();
	}

	return FReply::Handled();
}
