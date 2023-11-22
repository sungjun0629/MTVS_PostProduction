// Fill out your copyright notice in the Description page of Project Settings.


#include "SSequencerDetail.h"
#include "Framework/Text/SlateImageRun.h"
#include "Framework/Text/IRun.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "SoundConverterLogic.h"
#include "IPConfig.h"
#include "MemoDetailTable.h"

void SSequencerDetail::Construct(const FArguments& InArgs)
{
	FString sequenceName = IPConfig::SequenceName;

	IPConfig::sequnencerNameChanged.AddRaw(this,&SSequencerDetail::ReloadDetailPage);


	FString imagePath = GetImagePath(sequenceName);
	IPConfig::ImagePath = imagePath;

	USoundConverterLogic* ImageLibrary = NewObject<USoundConverterLogic>();
	//ImageLibrary->SearchImageFromUE(imagePath);

	const FSlateBrush* MyBrush = &(ImageLibrary->MySlateBrush);

	image = SNew(SImage)
			.Image(MyBrush);

	title = SNew(STextBlock);
	participants = SNew(STextBlock);
	content = SNew(STextBlock);

	sceneInfo = SNew(STextBlock);

	ReloadContent(sequenceName);



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
						title.ToSharedRef()
					]

					+ SVerticalBox::Slot()
						[
							SNew(STextBlock)
								.Text(FText::FromString("Content"))
						]

						+ SVerticalBox::Slot()
						[
							content.ToSharedRef()
						]
				]
			]

			+SVerticalBox::Slot()
			.Padding(25,10,0,30)
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
					[
					SNew(STextBlock)
					.Text(FText::FromString("Sequencer Info"))
				]
				
				+SVerticalBox::Slot()
					.Padding(0,5,0,0)
				[
						sceneInfo.ToSharedRef()
				]
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

void SSequencerDetail::ReloadContent(FString sequenceName)
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/Detail_Info.Detail_Info'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	TableRows.Empty();
	LoadedDataTable->GetAllRows<FMemoDetailTable>("random" , TableRows);

	for ( FMemoDetailTable* TableRow : TableRows )
	{
		if ( TableRow->sequenceTitle == sequenceName )
		{
			UE_LOG(LogTemp , Warning , TEXT("TableRow : %s") , *TableRow->content);
			title->SetText(FText::FromString(TableRow->sequenceTitle));
			participants->SetText(FText::FromString(TableRow->participants));
			content->SetText(FText::FromString(TableRow->content));
			sceneInfo->SetText(FText::FromString(TableRow->sequencerInfo));
		}
	}
}

FString SSequencerDetail::GetImagePath(FString sequenceName)
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/Detail_Info.Detail_Info'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	TableRows.Empty();
	LoadedDataTable->GetAllRows<FMemoDetailTable>("random" , TableRows);

	for ( FMemoDetailTable* TableRow : TableRows )
	{
		if ( TableRow->sequenceTitle == sequenceName )
		{
			return TableRow->imagePath;
		}
	}

	return "";
}

void SSequencerDetail::ReloadDetailPage(FString sequenceName)
{
	UE_LOG(LogTemp,Warning,TEXT("ReloadDetailPage Execute"));
	ReloadContent(sequenceName);
	IPConfig::ImagePath = GetImagePath(sequenceName);

	USoundConverterLogic* ImageLibrary = NewObject<USoundConverterLogic>();
	const FSlateBrush* MyBrush = &(ImageLibrary->MySlateBrush);
	image->SetImage(MyBrush);
}
