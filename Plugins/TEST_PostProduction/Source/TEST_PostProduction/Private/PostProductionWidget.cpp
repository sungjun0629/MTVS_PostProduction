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


void SPostProductionWidget::Construct(const FArguments& InArgs)
{
	// Set the content scale for this widget.
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 30;

	ChildSlot
		[	//Main vertical box
			SNew(SVerticalBox)

				//First vertical slot for title text
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0,80,0,50)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Login")))
						.Font(TitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]

				// Second vertical slot for username and password
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(100,0, 100,30)
				[
					SNew(SButton)
						.OnClicked(this, &SPostProductionWidget::OnKakaoLoginClicked)
						.HAlign(HAlign_Center)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Kakao Login"))
								.ColorAndOpacity(FSlateColor(FLinearColor::White))
						]
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(100, 0, 100, 30)
				[
					SNew(SButton)
						.Text(FText::FromString("GitHub Login"))
						.OnClicked(this, &SPostProductionWidget::OnGitHubLoginClicked)
						.HAlign(HAlign_Center)
				]

		];
}

FReply SPostProductionWidget::OnKakaoLoginClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Kakao Login Clicked"));
	return FReply::Handled();
}

FReply SPostProductionWidget::OnGitHubLoginClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("GitHub Login Clicked"));
	return FReply::Handled();
}
