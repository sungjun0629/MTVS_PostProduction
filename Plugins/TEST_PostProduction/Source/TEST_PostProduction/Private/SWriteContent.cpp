// Fill out your copyright notice in the Description page of Project Settings.


#include "SWriteContent.h"
#include "MemoDataTable.h"
#include "SequencerPractice.h"
#include "Templates/SharedPointer.h"
#include "SoundConverterLogic.h"

void SWriteContent::Construct(const FArguments& InArgs)
{
	USoundConverterLogic* soundConverterLogic = NewObject<USoundConverterLogic>();
	soundConverterLogic->GetSequenceAsset();
	Options = soundConverterLogic->Options;

	comboBoxContent = SNew(STextBlock).Text(FText::FromString("Sequencer"));

	ChildSlot
		[
			SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(ComboBoxWidget , SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&Options)
						.Content()
						[
							comboBoxContent.ToSharedRef()
						]
						.OnGenerateWidget_Lambda([] (TSharedPtr<FString> Item)
						{

							return SNew(STextBlock).Text(FText::FromString(*Item.Get()));
						})
						.OnSelectionChanged_Lambda([ = ] (TSharedPtr<FString> Item , ESelectInfo::Type SelectType)
						{// 중요, ListView refresh Logic
							if ( Item.IsValid() )
							{
								FString SelectedItem = *Item.Get();
								// Handle the selection here.
								UE_LOG(LogTemp , Warning , TEXT("Selected Item: %s") , *SelectedItem);
								sequenceName = SelectedItem;
								comboBoxContent->SetText(FText::FromString(SelectedItem));
							}
						})
				]


				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SEditableText)
						.Text(FText::FromString("Title"))
						.OnTextCommitted_Lambda([ = ] (const FText& InText , ETextCommit::Type InCommitType) {

						title = InText.ToString();
						UE_LOG(LogTemp , Warning , TEXT("Title : %s") , *InText.ToString());
						})

				]

				+ SVerticalBox::Slot()
				.MaxHeight(30.f)
				[
					SNew(SEditableText)
						.Text(FText::FromString("Content"))
						.OnTextCommitted_Lambda([ = ] (const FText& InText , ETextCommit::Type InCommitType) {

						content = InText.ToString();
						UE_LOG(LogTemp , Warning , TEXT("Content : %s") , *InText.ToString());
						})
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
						.Text(FText::FromString("Button"))
						.OnClicked(this , &SWriteContent::OnSubmitClicked)
				]
		];
}

FReply SWriteContent::OnSubmitClicked()
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";

	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	const FDateTime Now = FDateTime::Now();
	const FString DateTimeString = Now.ToString(TEXT("%Y/%m/%d/%H:%M:%S"));
	const FString UUID = Now.ToString(TEXT("%Y%m%d%H%M%S"));

	FMemoDataTable MemoDataTable;
	MemoDataTable.title = title;
	MemoDataTable.content = content;
	MemoDataTable.sequenceName = sequenceName;
	MemoDataTable.createdAt = DateTimeString;

	if ( LoadedDataTable )
	{
		// DataTable loaded successfully. You can now use the LoadedDataTable object.
		LoadedDataTable->AddRow(FName(UUID) , MemoDataTable);
	}
	else
	{
		// Failed to load DataTable. Handle the error.
	}

	return FReply::Handled();
}
