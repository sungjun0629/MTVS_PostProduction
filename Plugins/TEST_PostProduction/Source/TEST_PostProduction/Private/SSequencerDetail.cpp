// Fill out your copyright notice in the Description page of Project Settings.


#include "SSequencerDetail.h"

void SSequencerDetail::Construct(const FArguments& InArgs)
{

	title = SNew(STextBlock);
	period = SNew(STextBlock);
	author = SNew(STextBlock);

	sceneInfo = SNew(STextBlock);


	ChildSlot
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
				.Padding(25)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				[
					SNew(SImage)

				]

				// 작업기간 및 담당자 
				+ SHorizontalBox::Slot()
					.Padding(20)
					[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString("Title"))
					]

					+ SVerticalBox::Slot()
						[
						SNew(STextBlock)
						.Text(FText::FromString("Content"))
					]
				]
			]

			+SVerticalBox::Slot()
			.Padding(25,10,0,30)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Sequencer Info"))
			]

			+SVerticalBox::Slot()
				.Padding(25 , 10)
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
										SNew(SButton)
					.Text(FText::FromString("modify"))
				]
		];
}

void SSequencerDetail::ReloadContent()
{
	title->SetText(FText::FromString("Sequencer Info"));
	period->SetText(FText::FromString("Sequencer Info"));
	author->SetText(FText::FromString("Sequencer Info"));

	sceneInfo->SetText(FText::FromString("Sequencer Info"));
}
