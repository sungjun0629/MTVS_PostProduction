// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoTableEditor.h"
#include "Toolkits/BaseToolkit.h"
#include "Styling/AppStyle.h"
#include "Widgets/Docking/SDockTab.h"
#include "SMemoTableListViewRow.h"
#include "Templates/SharedPointer.h"
#include "MemoDataTable.h"
#include "UObject/UnrealType.h"
#include "Fonts/FontMeasure.h"
#include "DataTableUtils.h"
#include "IDocumentation.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Views/ITableRow.h"
#include "Editor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Widgets/Views/SHeaderRow.h"

FMemoTableEditor::FMemoTableEditor()
{

}

FMemoTableEditor::~FMemoTableEditor()
{
	
}

FName FMemoTableEditor::GetToolkitFName() const
{
	return FName("DataTableEditor");
}

FText FMemoTableEditor::GetBaseToolkitName() const
{
	return FText::FromString("DataTable Editor");
}

FString FMemoTableEditor::GetWorldCentricTabPrefix() const
{
	return "DataTable";
}

FLinearColor FMemoTableEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.0f , 0.0f , 0.2f , 0.5f);
}

void FMemoTableEditor::PostUndo(bool bSuccess)
{

}

void FMemoTableEditor::PostRedo(bool bSuccess)
{

}

void FMemoTableEditor::HandleUndoRedo()
{

}

void FMemoTableEditor::PreChange(const class UUserDefinedStruct* Struct , FStructureEditorUtils::EStructureEditorChangeInfo Info)
{

}

void FMemoTableEditor::PreChange(const UDataTable* Changed , FDataTableEditorUtils::EDataTableChangeInfo Info)
{

}

void FMemoTableEditor::PostChange(const class UUserDefinedStruct* Struct , FStructureEditorUtils::EStructureEditorChangeInfo Info)
{

}

void FMemoTableEditor::PostChange(const UDataTable* Changed , FDataTableEditorUtils::EDataTableChangeInfo Info)
{

}

void FMemoTableEditor::SelectionChange(const UDataTable* Changed , FName RowName)
{

}

FText FMemoTableEditor::GetCellText(TSharedPtr<FMemoDataTable> InRowDataPointer , int32 ColumnIndex) const
{
	if ( InRowDataPointer.IsValid() && ColumnIndex < InRowDataPointer->CellData.Num() )
	{
		//UE_LOG(LogTemp,Warning,TEXT("GetCellText : %d : %s"), ColumnIndex, *(InRowDataPointer->CellData[ ColumnIndex ].ToString()));
		return InRowDataPointer->CellData[ ColumnIndex ];
	}

	return FText();
}

FText FMemoTableEditor::GetFilterText() const
{
	return ActiveFilterText;
}

void FMemoTableEditor::OnFilterTextChanged(const FText& InFilterText)
{
	ActiveFilterText = InFilterText;
	UpdateVisibleRows();
}

void FMemoTableEditor::OnFilterTextCommitted(const FText& NewText , ETextCommit::Type CommitInfo)
{
	if ( CommitInfo == ETextCommit::OnCleared )
	{
		SearchBoxWidget->SetText(FText::GetEmpty());
		OnFilterTextChanged(FText::GetEmpty());
	}
}

void FMemoTableEditor::SetHighlightedRow(FName Name)
{
	if ( Name == HighlightedRowName )
	{
		return;
	}

	if ( Name.IsNone() )
	{
		HighlightedRowName = NAME_None;
		CellsListView->ClearSelection();
		HighlightedVisibleRowIndex = INDEX_NONE;
	}
	else
	{
		HighlightedRowName = Name;

		TSharedPtr<FMemoDataTable>* NewSelectionPtr = NULL;
		for ( HighlightedVisibleRowIndex = 0; HighlightedVisibleRowIndex < VisibleRows.Num(); ++HighlightedVisibleRowIndex )
		{
			if ( VisibleRows[ HighlightedVisibleRowIndex ]->RowId == Name )
			{
				NewSelectionPtr = &( VisibleRows[ HighlightedVisibleRowIndex ] );

				break;
			}
		}


		// Synchronize the list views
		if ( NewSelectionPtr )
		{
			CellsListView->SetSelection(*NewSelectionPtr);

			CellsListView->RequestScrollIntoView(*NewSelectionPtr);
		}
		else
		{
			CellsListView->ClearSelection();
		}
	}
}

TSharedRef<SDockTab> FMemoTableEditor::SpawnTab_DataTable(const FSpawnTabArgs& Args)
{
	//check(Args.GetTabId().TabType == DataTableTabId);

	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* Table = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	// Support undo/redo
	if ( Table )
	{
		Table->SetFlags(RF_Transactional);
	}

	return SNew(SDockTab)
		.TabColorScale(GetTabColorScale())
		[
			SNew(SBorder)
				.Padding(2)
				.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
				[
					DataTableTabWidget.ToSharedRef()
				]
		];
}

void FMemoTableEditor::CreateAndRegisterDataTableTab(const TSharedRef<class FTabManager>& InTabManager)
{
	DataTableTabWidget = CreateContentBox();

	InTabManager->RegisterTabSpawner(DataTableTabId , FOnSpawnTab::CreateRaw(this , &FMemoTableEditor::SpawnTab_DataTable))
		//.SetDisplayName(LOCTEXT("DataTableTab" , "Data Table"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
	InTabManager->TryInvokeTab(DataTableTabId);

}

TSharedRef<SVerticalBox> FMemoTableEditor::CreateContentBox()
{
	TSharedRef<SScrollBar> HorizontalScrollBar = SNew(SScrollBar)
		.Orientation(Orient_Horizontal)
		.Thickness(FVector2D(12.0f , 12.0f));

	TSharedRef<SScrollBar> VerticalScrollBar = SNew(SScrollBar)
		.Orientation(Orient_Vertical)
		.Thickness(FVector2D(12.0f , 12.0f));

	ColumnNamesHeaderRow = SNew(SHeaderRow);

	//VisibleRows.Add(TSharedPtr<FMemoDataTable>());

	CellsListView = SNew(SListView<TSharedPtr<FMemoDataTable>>)
		.ListItemsSource(&VisibleRows)
		.HeaderRow(ColumnNamesHeaderRow)
		.OnGenerateRow(this , &FMemoTableEditor::MakeRowWidget)
		.OnSelectionChanged(this , &FMemoTableEditor::OnRowSelectionChanged)
		.ExternalScrollbar(VerticalScrollBar)
		.ConsumeMouseWheel(EConsumeMouseWheel::Always)
		.SelectionMode(ESelectionMode::Single)
		.AllowOverscroll(EAllowOverscroll::No);

	RefreshCachedDataTable();

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SAssignNew(SearchBoxWidget , SSearchBox)
						.InitialText(this , &FMemoTableEditor::GetFilterText)
						.OnTextChanged(this , &FMemoTableEditor::OnFilterTextChanged)
						.OnTextCommitted(this , &FMemoTableEditor::OnFilterTextCommitted)
				]
		]
		+ SVerticalBox::Slot()
		[

			CellsListView.ToSharedRef()

			//SNew(SHorizontalBox)
			//	+ SHorizontalBox::Slot()

			//	[
			//		SNew(SScrollBox)
			//			.Orientation(Orient_Horizontal)
			//			.ExternalScrollbar(HorizontalScrollBar)
			//			+ SScrollBox::Slot()
			//			[
			//				CellsListView.ToSharedRef()
			//			]
			//	]
			//	+ SHorizontalBox::Slot()
			//	//.AutoWidth()
			//	[
			//		VerticalScrollBar
			//	]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					HorizontalScrollBar
				]
		];
}

TSharedRef<ITableRow> FMemoTableEditor::MakeRowWidget(TSharedPtr<FMemoDataTable> InRowDataPtr , const TSharedRef<STableViewBase>& OwnerTable)
{
	UE_LOG(LogTemp,Warning,TEXT("MakeRowWidget"))
	
	return
		SNew(SMemoTableListViewRow , OwnerTable)
		.DataTableEditor(SharedThis(this))
		.RowDataPtr(InRowDataPtr)
		.IsEditable(true);

}


void FMemoTableEditor::OnRowSelectionChanged(TSharedPtr<FMemoDataTable> InNewSelection , ESelectInfo::Type InSelectInfo)
{

	UE_LOG(LogTemp,Warning,TEXT("OnRowSelectionChanged"))
	const bool bSelectionChanged = !InNewSelection.IsValid() || InNewSelection->RowId != HighlightedRowName;
	const FName NewRowName = ( InNewSelection.IsValid() ) ? InNewSelection->RowId : NAME_None;

	SetHighlightedRow(NewRowName);

	if ( bSelectionChanged )
	{
		CallbackOnRowHighlighted.ExecuteIfBound(HighlightedRowName);
	}
}

void FMemoTableEditor::RefreshCachedDataTable(const FName InCachedSelection /*= NAME_None */ , const bool bUpdateEvenIfValid /*= false*/)
{
	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/NewDataTable.NewDataTable'";
	UDataTable* Table = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));

	// Table Column과 Row를 가져온다. 
	if ( Table )
	{
		GetDataForEditing(Table->RowStruct, Table->GetRowMap(), AvailableColumns, AvailableRows);
	}


	// Setup the default auto-sized columns
	ColumnWidths.SetNum(AvailableColumns.Num());
	for ( int32 ColumnIndex = 0; ColumnIndex < AvailableColumns.Num(); ++ColumnIndex )
	{
		const TSharedPtr<FMemoDataTableColumn>& ColumnData = AvailableColumns[ ColumnIndex ];
		FColumnWidth& ColumnWidth = ColumnWidths[ ColumnIndex ];
		ColumnWidth.CurrentWidth = FMath::Clamp(ColumnData->DesiredColumnWidth , 10.0f , 400.0f); // Clamp auto-sized columns to a reasonable limit
	}


	ColumnNamesHeaderRow->ClearColumns();

	//if ( true )
	//{
	//	ColumnNamesHeaderRow->AddColumn(
	//		SHeaderRow::Column(RowDragDropColumnId)
	//		[
	//			SNew(SBox)
	//				.VAlign(VAlign_Fill)
	//				.HAlign(HAlign_Fill)
	//			/*	.ToolTip(IDocumentation::Get()->CreateToolTip(
	//					LOCTEXT("DataTableRowHandleTooltip" , "Drag Drop Handles") ,
	//					nullptr ,
	//					*FDataTableEditorUtils::VariableTypesTooltipDocLink ,
	//					TEXT("DataTableRowHandle")))*/
	//				[
	//					SNew(STextBlock)
	//						.Text(FText::GetEmpty())
	//				]
	//		]
	//	);
	//}

	//ColumnNamesHeaderRow->AddColumn(
	//	SHeaderRow::Column(RowNumberColumnId)
	//	/*.SortMode(this , &FDataTableEditor::GetColumnSortMode , RowNumberColumnId)
	//	.OnSort(this , &FDataTableEditor::OnColumnNumberSortModeChanged)
	//	.ManualWidth(this , &FDataTableEditor::GetRowNumberColumnWidth)
	//	.OnWidthChanged(this , &FDataTableEditor::OnRowNumberColumnResized)*/
	//	[
	//		SNew(SBox)
	//			.VAlign(VAlign_Fill)
	//			.HAlign(HAlign_Fill)
	//			/*.ToolTip(IDocumentation::Get()->CreateToolTip(
	//				LOCTEXT("DataTableRowIndexTooltip" , "Row Index") ,
	//				nullptr ,
	//				*FDataTableEditorUtils::VariableTypesTooltipDocLink ,
	//				TEXT("DataTableRowIndex")))*/
	//			[
	//				SNew(STextBlock)
	//					.Text(FText::GetEmpty())
	//			]
	//	]

	//);

	//ColumnNamesHeaderRow->AddColumn(
	//	SHeaderRow::Column(RowNameColumnId)
	//	/*.DefaultLabel(LOCTEXT("DataTableRowName" , "Row Name"))
	//	.ManualWidth(this , &FDataTableEditor::GetRowNameColumnWidth)
	//	.OnWidthChanged(this , &FDataTableEditor::OnRowNameColumnResized)
	//	.SortMode(this , &FDataTableEditor::GetColumnSortMode , RowNameColumnId)
	//	.OnSort(this , &FDataTableEditor::OnColumnNameSortModeChanged)*/
	//);

	for ( int32 ColumnIndex = 0; ColumnIndex < AvailableColumns.Num(); ++ColumnIndex )
	{
		const TSharedPtr<FMemoDataTableColumn>& ColumnData = AvailableColumns[ ColumnIndex ];

		ColumnNamesHeaderRow->AddColumn(
			SHeaderRow::Column(ColumnData->ColumnId)
			.DefaultLabel(ColumnData->DisplayName)
		/*	.ManualWidth(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateSP(this , &FDataTableEditor::GetColumnWidth , ColumnIndex)))
			.OnWidthChanged(this , &FDataTableEditor::OnColumnResized , ColumnIndex)*/
			.SortMode(this , &FMemoTableEditor::GetColumnSortMode , ColumnData->ColumnId)
			.OnSort(this , &FMemoTableEditor::OnColumnSortModeChanged)
			[
				SNew(SBox)
					.Padding(FMargin(0 , 4 , 0 , 4))
					.VAlign(VAlign_Fill)
				/*	.ToolTip(IDocumentation::Get()->CreateToolTip(FDataTableEditorUtils::GetRowTypeInfoTooltipText(ColumnData) , nullptr , *FDataTableEditorUtils::VariableTypesTooltipDocLink , FDataTableEditorUtils::GetRowTypeTooltipDocExcerptName(ColumnData)))*/
					[
						SNew(STextBlock)
							.Justification(ETextJustify::Center)
							.Text(ColumnData->DisplayName)
					]
			]
		);
	}

	UpdateVisibleRows(InCachedSelection , bUpdateEvenIfValid);


}	

void FMemoTableEditor::UpdateVisibleRows(const FName InCachedSelection /*= NAME_None */ , const bool bUpdateEvenIfValid /*= false*/)
{
	if ( ActiveFilterText.IsEmptyOrWhitespace() )
	{
		VisibleRows = AvailableRows;
		UE_LOG(LogTemp,Warning,TEXT("UpdateVisibleRows_ActiveFilter"))
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("UpdateVisibleRows_ActiveFilter.else"))
		VisibleRows.Empty(AvailableRows.Num());

		const FString& ActiveFilterString = ActiveFilterText.ToString();
		for ( const TSharedPtr<FMemoDataTable>& RowData : AvailableRows )
		{
			bool bPassesFilter = false;

			if ( RowData->DisplayName.ToString().Contains(ActiveFilterString) )
			{
				bPassesFilter = true;
			}
			else
			{
				for ( const FText& CellText : RowData->CellData )
				{
					if ( CellText.ToString().Contains(ActiveFilterString) )
					{
						bPassesFilter = true;
						break;
					}
				}
			}

			if ( bPassesFilter )
			{
				VisibleRows.Add(RowData);
			}
		}
	}

	CellsListView->RequestListRefresh();
	RestoreCachedSelection(InCachedSelection , bUpdateEvenIfValid);
}

void FMemoTableEditor::RestoreCachedSelection(const FName InCachedSelection , const bool bUpdateEvenIfValid /*= false*/)
{

}

void FMemoTableEditor::GetDataForEditing(const UScriptStruct* RowStruct , const TMap<FName , uint8*>& RowMap , TArray<TSharedPtr<FMemoDataTableColumn>>& OutAvailableColumns , TArray<TSharedPtr<FMemoDataTable>>& OutAvailableRows)
{
	TArray<TSharedPtr<FMemoDataTableColumn>> OldColumns = OutAvailableColumns;
	TArray<TSharedPtr<FMemoDataTable>> OldRows = OutAvailableRows;


	// First build array of properties
	TArray<const FProperty*> StructProps;
	for ( TFieldIterator<const FProperty> It(RowStruct); It; ++It )
	{
		const FProperty* Prop = *It;
		check(Prop);
		if ( !Prop->HasMetaData(FName(TEXT("HideFromDataTableEditorColumn"))) )
		{
			StructProps.Add(Prop);
		}
	}

	TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	const FTextBlockStyle& CellTextStyle = FAppStyle::GetWidgetStyle<FTextBlockStyle>("DataTableEditor.CellText");
	static const float CellPadding = 10.0f;

	// Populate the column data
	OutAvailableColumns.Reset(StructProps.Num());
	for ( int32 Index = 0; Index < StructProps.Num(); ++Index )
	{
		const FProperty* Prop = StructProps[ Index ];
		const FText PropertyDisplayName = DataTableUtils::GetPropertyDisplayName(Prop , FName::NameToDisplayString(Prop->GetName() , Prop->IsA<FBoolProperty>()));

		TSharedPtr<FMemoDataTableColumn> CachedColumnData;

		// If at all possible, attempt to reuse previous columns if their data has not changed
		if ( Index >= OldColumns.Num() || OldColumns[ Index ]->ColumnId != Prop->GetFName() || !OldColumns[ Index ]->DisplayName.EqualTo(PropertyDisplayName) )
		{
			CachedColumnData = MakeShareable(new FMemoDataTableColumn());
			CachedColumnData->ColumnId = Prop->GetFName();
			CachedColumnData->DisplayName = PropertyDisplayName;
			CachedColumnData->Property = Prop;
		}
		else
		{
			CachedColumnData = OldColumns[ Index ];

			// Need to update property hard pointer in case it got reconstructed
			CachedColumnData->Property = Prop;
		}

		CachedColumnData->DesiredColumnWidth = static_cast< float >( FontMeasure->Measure(CachedColumnData->DisplayName , CellTextStyle.Font).X + CellPadding );

		OutAvailableColumns.Add(CachedColumnData);
	}


	// Populate the row data
	OutAvailableRows.Reset(RowMap.Num());
	int32 Index = 0;
	for ( auto RowIt = RowMap.CreateConstIterator(); RowIt; ++RowIt , ++Index )
	{
		FText RowName = FText::FromName(RowIt->Key);
		TSharedPtr<FMemoDataTable> CachedRowData;

		// If at all possible, attempt to reuse previous rows if their data has not changed.
		if ( Index >= OldRows.Num() || OldRows[ Index ]->RowId != RowIt->Key || !OldRows[ Index ]->DisplayName.EqualTo(RowName) )
		{
			CachedRowData = MakeShareable(new FMemoDataTable());
			CachedRowData->RowId = RowIt->Key;
			CachedRowData->DisplayName = RowName;
			CachedRowData->CellData.Reserve(StructProps.Num());
		}
		else
		{
			CachedRowData = OldRows[ Index ];
			CachedRowData->CellData.Reset(StructProps.Num());
		}

		CachedRowData->DesiredRowHeight = FontMeasure->GetMaxCharacterHeight(CellTextStyle.Font);
		CachedRowData->RowNum = Index + 1;

		// Always rebuild cell data
		{
			const uint8* RowData = RowIt.Value();
			for ( int32 ColumnIndex = 0; ColumnIndex < StructProps.Num(); ++ColumnIndex )
			{
				const FProperty* Prop = StructProps[ ColumnIndex ];
				TSharedPtr<FMemoDataTableColumn> CachedColumnData = OutAvailableColumns[ ColumnIndex ];

				const FText CellText = DataTableUtils::GetPropertyValueAsText(Prop , RowData);
				CachedRowData->CellData.Add(CellText);

				const FVector2D CellTextSize = FontMeasure->Measure(CellText , CellTextStyle.Font);

				CachedRowData->DesiredRowHeight = static_cast< float >( FMath::Max(CachedRowData->DesiredRowHeight , CellTextSize.Y) );

				const float CellWidth = static_cast< float >( CellTextSize.X + CellPadding );
				CachedColumnData->DesiredColumnWidth = FMath::Max(CachedColumnData->DesiredColumnWidth , CellWidth);
			}
		}

		OutAvailableRows.Add(CachedRowData);
	}
}

EColumnSortMode::Type FMemoTableEditor::GetColumnSortMode(const FName ColumnId) const
{
	if ( SortByColumn != ColumnId )
	{
		return EColumnSortMode::None;
	}

	return SortMode;
}

void FMemoTableEditor::OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority , const FName& ColumnId , const EColumnSortMode::Type InSortMode)
{
	int32 ColumnIndex;

	SortMode = InSortMode;
	SortByColumn = ColumnId;

	for ( ColumnIndex = 0; ColumnIndex < AvailableColumns.Num(); ++ColumnIndex )
	{
		if ( AvailableColumns[ ColumnIndex ]->ColumnId == ColumnId )
		{
			break;
		}
	}

	if ( AvailableColumns.IsValidIndex(ColumnIndex) )
	{
		if ( InSortMode == EColumnSortMode::Ascending )
		{
			VisibleRows.Sort([ ColumnIndex ] (const TSharedPtr<FMemoDataTable>& first , const TSharedPtr<FMemoDataTable>& second)
			{
				int32 Result = ( first->CellData[ ColumnIndex ].ToString() ).Compare(second->CellData[ ColumnIndex ].ToString());

				if ( !Result )
				{
					return first->RowNum < second->RowNum;

				}

				return Result < 0;
			});

		}
		else if ( InSortMode == EColumnSortMode::Descending )
		{
			VisibleRows.Sort([ ColumnIndex ] (const TSharedPtr<FMemoDataTable>& first , const TSharedPtr<FMemoDataTable>& second)
			{
				int32 Result = ( first->CellData[ ColumnIndex ].ToString() ).Compare(second->CellData[ ColumnIndex ].ToString());

				if ( !Result )
				{
					return first->RowNum > second->RowNum;
				}

				return Result > 0;
			});
		}
	}

	CellsListView->RequestListRefresh();
}

void FMemoTableEditor::OnColumnNameSortModeChanged(const EColumnSortPriority::Type SortPriority , const FName& ColumnId , const EColumnSortMode::Type InSortMode)
{
	SortMode = InSortMode;
	SortByColumn = ColumnId;

	if ( InSortMode == EColumnSortMode::Ascending )
	{
		VisibleRows.Sort([] (const TSharedPtr<FMemoDataTable>& first , const TSharedPtr<FMemoDataTable>& second)
		{
			return ( first->DisplayName ).ToString() < ( second->DisplayName ).ToString();
		});
	}
	else if ( InSortMode == EColumnSortMode::Descending )
	{
		VisibleRows.Sort([] (const TSharedPtr<FMemoDataTable>& first , const TSharedPtr<FMemoDataTable>& second)
		{
			return ( first->DisplayName ).ToString() > ( second->DisplayName ).ToString();
		});
	}

	CellsListView->RequestListRefresh();
}
