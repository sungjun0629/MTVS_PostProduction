// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerPractice.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "Engine/DataTable.h"
#include "MemoDataTable.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Views/SListView.h"

void SSequencePractice::Construct(const FArguments& InArgs)
{
	GetSequenceAsset();

	contentTitle = SNew(STextBlock)
		.Text(FText::FromString("Sequencer"));


	TArray<FMemoDataTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.


	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";

	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));
	LoadedDataTable->GetAllRows<FMemoDataTable>(comboBoxContent,TableRows);


	TArray<TSharedPtr<FMemoDataTable>> TableRowsPtrArray; // Assuming FMyDataTableType is the struct type of your DataTable rows.
	for ( FMemoDataTable* TableRow : TableRows )
	{
		TableRowsPtrArray.Add(MakeShareable(TableRow));
		UE_LOG(LogTemp , Warning , TEXT("TableRow : %s") , *TableRow->content);
	}

	// Create an SListView widget to display the DataTable content.
	TSharedPtr<SListView<TSharedPtr<FMemoDataTable>>> ListView;


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
						.OnTextCommitted_Lambda([=](const FText& InText, ETextCommit::Type InCommitType) { 
						
							title = InText.ToString();
							UE_LOG(LogTemp, Warning, TEXT("Title : %s"), *InText.ToString()); 
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

				+SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				[
					SNew(SButton)
						.Text(FText::FromString("Button"))
						.OnClicked(this, &SSequencePractice::OnSubmitClicked)
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(ComboBoxWidget , SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&Options)
						.Content()
						[
							SNew(STextBlock)
								.Text(FText::FromString("Sequencer"))
						]
						.OnGenerateWidget_Lambda([] (TSharedPtr<FString> Item)
						{
												
												return SNew(STextBlock).Text(FText::FromString(*Item.Get()));
						})
						.OnSelectionChanged_Lambda([=] (TSharedPtr<FString> Item , ESelectInfo::Type SelectType)
						{
							if ( Item.IsValid() )
							{
								FString SelectedItem = *Item.Get();
								// Handle the selection here.
								UE_LOG(LogTemp , Warning , TEXT("Selected Item: %s") , *SelectedItem);
								
								ChangeContent(SelectedItem);
								
							}
						})
					
					//SNew(SComboBox<TSharedPtr<FString>>)
					//.Content()
					//[
					//	SNew(STextBlock)
					//	.Text(FText::FromString("Sequencer"))
					//]
					//.OptionsSource(&Options)
					//	.OnGenerateWidget_Lambda([] (TSharedPtr<FString> Item)
					//	{
					//			return SNew(STextBlock).Text(FText::FromString(*Item.Get()));
					//	})
					//.OnSelectionChanged_Lambda([=] (TSharedPtr<FString> Item , ESelectInfo::Type SelectType)
					//	{
					//		if ( Item.IsValid() )
					//		{
					//			FString SelectedItem = *Item.Get();
					//			// Handle the selection here.
					//			UE_LOG(LogTemp , Warning , TEXT("Selected Item: %s") , *SelectedItem);
					//			
					//			//ComboBoxWidget->SetContent(SNew(STextBlock).Text(FText::FromString(SelectedItem)));
					//			//ComboBoxWidget->RefreshOptions();
					//		}
					//	})
				]

				+SVerticalBox::Slot()
				[
					SAssignNew(ListView , SListView<TSharedPtr<FMemoDataTable>>)
						.ListItemsSource(&TableRowsPtrArray)
						.OnGenerateRow_Lambda([] (TSharedPtr<FMemoDataTable> Item , const TSharedRef<STableViewBase>& OwnerTable)
						{
							return SNew(STableRow<TSharedPtr<FMemoDataTable>> , OwnerTable)
								.Padding(1)
								.Content()
								[
									SNew(STextBlock).Text(FText::FromString(Item->content)) // Display the name property, modify as needed.
								];
						})

				]
			
		];


}

void SSequencePractice::GetSequenceAsset()
{
	// Search Test
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();


	TArray<FAssetData> AssetList;
	//AssetRegistry.GetAssetsByClass(ULevelSequence::StaticClass()->GetFName(), AssetList);
	AssetRegistry.GetAssetsByPath(FName("/Game/Sungjun/Sequence/"), AssetList);

	for (const FAssetData& Asset : AssetList)
	{
		// Use the asset as needed
		ULevelSequence* LevelSequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (LevelSequence)
		{
			// Perform operations on the Level Sequence
			UE_LOG(LogTemp, Warning, TEXT("LevelSequence : %s"), *LevelSequence->GetName());
			Options.Add(MakeShareable(new FString(LevelSequence->GetName())));
		}
	}
}

void SSequencePractice::ChangeContent(FString contentString)
{
	//ComboBoxWidget->SetMenuContent(SNew(STextBlock).Text(FText::FromString(contentString)));
	//ComboBoxWidget->SetContent(SNew(STextBlock).Text(FText::FromString(contentString)));

	UE_LOG(LogTemp,Warning,TEXT("ChangeContent"));
	contentTitle->SetText(FText::FromString(contentString));

	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";

	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));


	TArray<FMemoDataTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.
	// Populate the TableRows array with data from the DataTable.


	
	// Create an SListView widget to display the DataTable content.
	TSharedPtr<SListView<TSharedPtr<FMemoDataTable>>> ListView;
	SAssignNew(ListView , SListView<TSharedPtr<FMemoDataTable>>)
		//.ItemList(TableRows)
		.OnGenerateRow_Lambda([] (TSharedPtr<FMemoDataTable> Item , const TSharedRef<STableViewBase>& LoadedDataTable)
		{
				return SNew(STableRow<TSharedPtr<FMemoDataTable>> , LoadedDataTable)
					.Padding(1)
					.Content()
					[
						SNew(STextBlock).Text(FText::FromString(Item->content)) // Display the name property, modify as needed.
					];
		});
}

FReply SSequencePractice::OnSubmitClicked()
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";

	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	FMemoDataTable MemoDataTable;
	MemoDataTable.title = title;
	MemoDataTable.content = content;
	MemoDataTable.createdAt = "1";

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


