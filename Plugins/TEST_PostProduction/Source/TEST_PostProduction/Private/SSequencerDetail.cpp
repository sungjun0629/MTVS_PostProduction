// Fill out your copyright notice in the Description page of Project Settings.


#include "SSequencerDetail.h"
#include "Framework/Text/SlateImageRun.h"
#include "Framework/Text/IRun.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "SoundConverterLogic.h"

void SSequencerDetail::Construct(const FArguments& InArgs)
{

	USoundConverterLogic* ImageLibrary = NewObject<USoundConverterLogic>();

	const FSlateBrush* MyBrush = &(ImageLibrary->MySlateBrush);

	image = SNew(SImage)
			.Image(MyBrush);

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

				// 스토리보드 사진
				+ SHorizontalBox::Slot()
				[
					image.ToSharedRef()

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

void SSequencerDetail::ReloadContent(FString _title, FString _period, FString _author, FString _sceneInfo)
{
	


	title->SetText(FText::FromString(_title));
	period->SetText(FText::FromString(_period));
	author->SetText(FText::FromString(_author));

	sceneInfo->SetText(FText::FromString(_sceneInfo));
}
