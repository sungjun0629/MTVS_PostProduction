// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerPractice.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "Engine/DataTable.h"
#include "MemoDataTable.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableViewBase.h"
#include "Framework/Docking/TabManager.h"
#include "SSequencerDetail.h"
#include "DataTablePractice.h"
#include "Widgets/Views/SHeaderRow.h"
#include "IDocumentation.h"
#include "PropertyEditorModule.h"
#include "Widgets/Input/SSearchBox.h"
#include "IPConfig.h"
#include "ImportExportDataTable.h"

void SSequencePractice::Construct(const FArguments& InArgs)
{
	GetSequenceAsset();

	contentTitle = SNew(STextBlock)
		.Text(FText::FromString("Sequencer"));


	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));
	//TArray<FMemoDataTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.
	LoadedDataTable->GetAllRows<FMemoDataTable>("random", TableRows);

	// 
	TArray<FName> RowNames = LoadedDataTable->GetRowNames();

	//for(FName RowName : RowNames)
	//{
	//	FTableRowBase* RowData = LoadedDataTable->FindRow<FTableRowBase>(RowName , "");

	//	// Create a property handle for the row data
	//	TSharedPtr<IPropertyHandle> RowHandle = FPropertyEditorModule::GetModule().CreatePropertyHandle(
	//		RowData->GetClass() ,
	//		RowName ,
	//		RowData);

	//	// Create a details view to convert the row data to FDataTableEditorRowListViewDataPtr
	//	TSharedRef<IDetailsView> DetailsView = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(
	//		FDetailsViewArgs()
	//	);

	//	DetailsView->SetObject(RowHandle->GetParentHandle()->GetProperty()->GetOwner());

	//	// Create FDataTableEditorRowListViewDataPtr from the row data
	//	FDataTableEditorRowListViewDataPtr RowListViewData = MakeShared<FDataTableEditorRowListViewData>();
	//	RowListViewData->Initialize(RowHandle);

	//} 

	//
	
	
	//class FDataTablePractice* DataTablePractice = new FDataTablePractice();

	for ( FMemoDataTable* TableRow : TableRows )
	{
		UE_LOG(LogTemp,Warning,TEXT("TableRow : %s"),*TableRow->title);
		memoItems.Add(MakeShareable(new FMemoDataTable(*TableRow)));
	}

	contentTitle = SNew(STextBlock).Text(FText::FromString("Sequencer"));

	ColumnNamesHeaderRow = SNew(SHeaderRow);

	ChildSlot
		[
			SNew(SVerticalBox)
			

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					[ 
						SAssignNew(ComboBoxWidget , SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&Options)
						.Content()
						[
							contentTitle.ToSharedRef()
						]
						.OnGenerateWidget_Lambda([] (TSharedPtr<FString> Item)
						{

							return SNew(STextBlock).Text(FText::FromString(*Item.Get()));
						})
						.OnSelectionChanged_Lambda([ = ] (TSharedPtr<FString> Item , ESelectInfo::Type SelectType)
						{// 중요, ListView refresh Logic
							if ( Item.IsValid() )
							{
								SelectedItem = *Item.Get();
								// Handle the selection here.
								UE_LOG(LogTemp , Warning , TEXT("Selected Item: %s") , *SelectedItem);

								contentTitle->SetText(FText::FromString(SelectedItem));
								ChangeContent(SelectedItem);
							}
						})
					]

					+SHorizontalBox::Slot()
						.AutoWidth()
					[
						SNew(SButton)
							.Text(FText::FromString("Detail"))
							.OnClicked(this , &SSequencePractice::OnDetailClicked)
					]

					+ SHorizontalBox::Slot()
						.AutoWidth()
					[
						SNew(SButton)
							.Text(FText::FromString("Write"))
							.OnClicked(this , &SSequencePractice::OnWriteClicked)
					]
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SAssignNew(SearchBoxWidget , SSearchBox)
								/*.InitialText(this , &FDataTablePractice::GetFilterText)
								.OnTextChanged(this , &FDataTablePractice::OnFilterTextChanged)
								.OnTextCommitted(this , &FDataTablePractice::OnFilterTextCommitted)*/
						]
				]



				+ SVerticalBox::Slot()
				[

					// FMemoDataTable
					SAssignNew(csvListView, SListView<TSharedPtr<FMemoDataTable>>)
					.HeaderRow(ColumnNamesHeaderRow)
					// Row의 자료형을 체크한다?
					.ListItemsSource(&memoItems)
					.OnMouseButtonDoubleClick(this, &SSequencePractice::OnMousebuttonDoubleClick)
					.SelectionMode(ESelectionMode::Single)
					.OnGenerateRow_Lambda([] (TSharedPtr<FMemoDataTable> Item , const TSharedRef<STableViewBase>& OwnerTable)
					{
						return SNew(STableRow<TSharedPtr<FMemoDataTable>> , OwnerTable)
							.Padding(1)
							.ShowWires(true)
							.Content()
							[
								SNew(SHorizontalBox)

									+ SHorizontalBox::Slot()
									.AutoWidth()
									[
										SNew(STextBlock).Text(FText::FromString(Item->title))
									]

									+ SHorizontalBox::Slot()
									[
										SNew(STextBlock).Text(FText::FromString(Item->content))
									]

									+ SHorizontalBox::Slot()
									.AutoWidth()
									[
										SNew(SCheckBox)
											.IsChecked(ECheckBoxState::Unchecked)
									]
							];
					})
				]

				+ SVerticalBox::Slot()
.AutoHeight()
.HAlign(HAlign_Right)
				[
					SNew(SButton)
						.Text(FText::FromString("Export"))
						.OnClicked(this , &SSequencePractice::OnExportClicked)
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

	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	IPConfig::SequenceName = contentString;
	filterSequenceName = contentString;
	TableRows.Empty();
	LoadedDataTable->GetAllRows<FMemoDataTable>("random" , TableRows);
	memoItems.Empty();
	for ( FMemoDataTable* TableRow : TableRows )
	{
		UE_LOG(LogTemp , Warning , TEXT("TableRow : %s") , *TableRow->title);
		if ( TableRow->sequenceName == filterSequenceName )
		memoItems.Add(MakeShareable(new FMemoDataTable(*TableRow)));
	}

	csvListView->RequestListRefresh();
	
}

void SSequencePractice::OnMousebuttonDoubleClick(TSharedPtr<FMemoDataTable> Item)
{
	UE_LOG(LogTemp , Warning , TEXT("OnMousebuttonDoubleClick : %s") , *Item->sequenceName);



}

FReply SSequencePractice::OnSubmitClicked()
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

FReply SSequencePractice::OnDetailClicked()
{

	TSharedPtr<SDockTab> checkTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("Detail Tab"));

	if ( !checkTab.IsValid() )
	{
		TSharedPtr<SDockTab> sequencerDetail = FGlobalTabmanager::Get()->TryInvokeTab(FName("Detail Tab"));
	}
	else
	{
		/*TSharedPtr<SSequencerDetail> customSharedPtr = StaticCastSharedPtr<SSequencerDetail>(checkTab);
		customSharedPtr->ReloadContent();*/
	}

	// 해당 Scene에 대한 정보로 갱신을 해준다. 
	// 해당 Scene에 대한 정보도 어딘가에 저장이 되어있어야 한다. -> Comment에 넣어주는 것보다 새로 DataTable을 만든다?


	return FReply::Handled();
}

FReply SSequencePractice::OnWriteClicked()
{
	//FDataTableEditor* DataTableEditor = NewObject<FDataTableEditor>();

	/*
	DataTableEditor->RegisterTabSpawners(FGlobalTabmanager::Get());
	DataTableEditor->InitDataTableEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), nullptr);*/

	//FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	////TSharedPtr<IToolkitHost> ToolkitHost = FToolkitManager::Get().FindEditorForAsset(this);

	//UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));
	//FDataTableEditorModule* DataTableEditorModule = &FModuleManager::LoadModuleChecked<FDataTableEditorModule>("DataTableEditor");
	//DataTableEditorModule->CreateDataTableEditor(EToolkitMode::Standalone , TSharedPtr<IToolkitHost>() , LoadedDataTable);

	//FDataTableEditor* DataTableEditor = new FDataTableEditor();
	//DataTableEditor->SpawnTab_DataTable();

	FGlobalTabmanager::Get()->TryInvokeTab(FName("MemoWrite Tab"));

	// Column Test
	/*FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	TArray<FString> AvailableColumns = LoadedDataTable->GetColumnTitles();

	ColumnNamesHeaderRow->AddColumn(
				SHeaderRow::Column("RowDragDropColumnId")
				[
					SNew(SBox)
						.VAlign(VAlign_Fill)
						.HAlign(HAlign_Fill)
						.ToolTip(IDocumentation::Get()->CreateToolTip(
							LOCTEXT("DataTableRowHandleTooltip" , "Drag Drop Handles") ,
							nullptr ,
							*FDataTableEditorUtils::VariableTypesTooltipDocLink ,
							TEXT("DataTableRowHandle")))
						[
							SNew(STextBlock)
								.Text(FText::GetEmpty())
						]
				]
	);

	for ( int32 ColumnIndex = 0; ColumnIndex < AvailableColumns.Num(); ++ColumnIndex )
	{
		const FString& ColumnData = AvailableColumns[ ColumnIndex ];

		ColumnNamesHeaderRow->AddColumn(
			SHeaderRow::Column(FName(ColumnData))
			.OnSort(this, &SSequencePractice::OnColumnNumberSortModeChanged)
			[
				SNew(SBox)
					.Padding(FMargin(0 , 4 , 0 , 4))
					.VAlign(VAlign_Fill)
					[
						SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Text(FText::FromString(ColumnData))
					]
			]
		);
	}*/

	return FReply::Handled();
}

FReply SSequencePractice::OnExportClicked()
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";

	UE_LOG(LogTemp , Warning , TEXT("OnExportClicked"));
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));
	bool isSuccess = false;
	FString OutInfoMessage;
	UExcelImportExportDataTable::ExportDataTableToCSV("D:\\DownTest\\Test.csv", LoadedDataTable, isSuccess, OutInfoMessage);

	return FReply::Handled();
}

void SSequencePractice::OnColumnNumberSortModeChanged(const EColumnSortPriority::Type SortPriority , const FName& ColumnId , const EColumnSortMode::Type InSortMode)
{
	UE_LOG(LogTemp , Warning , TEXT("OnColumnNumberSortModeChanged"));
}

