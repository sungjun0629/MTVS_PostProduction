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
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"

SSequencerDetail::~SSequencerDetail()
{
	IPConfig::sequnencerNameChanged.Clear();	
}

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

	title = SNew(SEditableText).IsEnabled(false).OnTextCommitted(this, &SSequencerDetail::OnTitleCommited);
	participants = SNew(STextBlock);
	content = SNew(SEditableText).IsEnabled(false).OnTextCommitted(this , &SSequencerDetail::OnContentCommited);

	sceneInfo = SNew(SEditableText).IsEnabled(false).OnTextCommitted(this , &SSequencerDetail::OnInfoCommited);
	ReloadContent(sequenceName);

	buttonText =SNew(STextBlock).Text(FText::FromString("modify"));


	ChildSlot
		[
			SNew(SHorizontalBox)


			// 스토리보드 사진
			+SHorizontalBox::Slot()
			.Padding(30)
			[
				SAssignNew(imageButton, SButton)
				.OnClicked(this,&SSequencerDetail::OnimageButtonClicked)
				.IsEnabled(false)
				[
					image.ToSharedRef()
				]

			]


			+ SHorizontalBox::Slot()
				.Padding(10)
			[
				SNew(SVerticalBox)


					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(5)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Title"))
					]

					+ SVerticalBox::Slot()
					.Padding(5)
					[
						SNew(SBorder)
.						BorderBackgroundColor(FLinearColor::Black)
						[
							title.ToSharedRef()
							
						]
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(5)
					[
						SNew(STextBlock)
							.Text(FText::FromString("Content"))
					]

					+ SVerticalBox::Slot()
					.Padding(5)
					[
						SNew(SBorder)
							.BorderBackgroundColor(FLinearColor::Black)
							[
								content.ToSharedRef()

							]
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(5)
					[
						SNew(STextBlock)
							.Text(FText::FromString("Sequencer Info"))
							
					]
				
					+ SVerticalBox::Slot()
					.Padding(5)
					[
						SNew(SBorder)
							.BorderBackgroundColor(FLinearColor::Black)
							[
								sceneInfo.ToSharedRef()
							]
					]

					+ SVerticalBox::Slot()
					.Padding(25 , 10)
					.HAlign(HAlign_Right)
					.AutoHeight()
					[
						SNew(SButton)
							.OnClicked(this , &SSequencerDetail::OnModifyButtonClicked)
							[
								buttonText.ToSharedRef()
							]
					]
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

	detailSequenceName = sequenceName;

	TableRows.Empty();
	LoadedDataTable->GetAllRows<FMemoDetailTable>("random" , TableRows);

	for ( FMemoDetailTable* TableRow : TableRows )
	{
		if ( TableRow->sequenceTitle == sequenceName )
		{
			imageRePath = TableRow->imagePath;
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

void SSequencerDetail::OnTitleCommited(const FText& Intext , ETextCommit::Type type)
{
	title->SetText(Intext);
}

void SSequencerDetail::OnContentCommited(const FText& Intext , ETextCommit::Type type)
{
	content->SetText(Intext);
}

void SSequencerDetail::OnInfoCommited(const FText& Intext , ETextCommit::Type type)
{
	sceneInfo->SetText(Intext);
}

FReply SSequencerDetail::OnModifyButtonClicked()
{
	if(!isEditable )
	{
		isEditable = true;
		title->SetEnabled(true);
		content->SetEnabled(true);
		sceneInfo->SetEnabled(true);
		imageButton->SetEnabled(true);
		buttonText->SetText(FText::FromString("submit"));
	}
	else {

		FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/Detail_Info.Detail_Info'";
		UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

		if(LoadedDataTable )
		{
			LoadedDataTable->GetAllRows<FMemoDetailTable>("random" , TableRows);

			for ( FMemoDetailTable* TableRow : TableRows )
			{
				if ( TableRow->sequenceTitle == detailSequenceName )
				{
					TableRow->sequenceTitle = title->GetText().ToString();
					TableRow->content = content->GetText().ToString();
					TableRow->sequencerInfo = sceneInfo->GetText().ToString();
					TableRow->imagePath = imageRePath;
				}
			}

			ReloadDetailPage(detailSequenceName);
		}

		isEditable = false;
		title->SetEnabled(false);
		content->SetEnabled(false);
		sceneInfo->SetEnabled(false);
		imageButton->SetEnabled(false);
		buttonText->SetText(FText::FromString("modify"));

	}

	return FReply::Handled();
}

FReply SSequencerDetail::OnimageButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if ( DesktopPlatform )
	{
		// Set the default path to the project directory
		FString DefaultPath = FPaths::ProjectDir();

		// Open the folder picker dialog
		const void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();
		TArray<FString> OutFolderNames;

		bool bOpened = DesktopPlatform->OpenFileDialog(
			ParentWindowPtr ,
			TEXT("Select a folder") ,
			DefaultPath ,
			TEXT("") ,
			TEXT("All Files (*.*)|*.*") ,
			EFileDialogFlags::None ,
			OutFolderNames
		);

		// Process the selected folder
		if ( bOpened && OutFolderNames.Num() > 0 )
		{
			FString SelectedDirectory = OutFolderNames[ 0 ];
			imageRePath = SelectedDirectory;
			// You can perform further actions with the selected directory here.
		}
	}


	return FReply::Handled();
}
