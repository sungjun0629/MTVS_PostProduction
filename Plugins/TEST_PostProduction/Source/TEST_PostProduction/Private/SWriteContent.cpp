// Fill out your copyright notice in the Description page of Project Settings.


#include "SWriteContent.h"
#include "MemoDataTable.h"

void SWriteContent::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SEditableText)
						.Text(FText::FromString("Sequence Name"))
						.OnTextCommitted_Lambda([ = ] (const FText& InText , ETextCommit::Type InCommitType) {

						sequenceName = InText.ToString();
						UE_LOG(LogTemp , Warning , TEXT("sequenceName : %s") , *InText.ToString());
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


	FMemoDataTable MemoDataTable;
	MemoDataTable.title = title;
	MemoDataTable.content = content;
	MemoDataTable.createdAt = DateTimeString;

	if ( LoadedDataTable )
	{
		// DataTable loaded successfully. You can now use the LoadedDataTable object.
		LoadedDataTable->AddRow(FName(sequenceName) , MemoDataTable);
	}
	else
	{
		// Failed to load DataTable. Handle the error.
	}

	return FReply::Handled();
}
