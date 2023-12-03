// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableEditor/Public/IDataTableEditor.h"
#include "MemoDataTable.h"
#include "Widgets/Views/ITableRow.h"
#include "EditorUndoClient.h"
#include "Kismet2/StructureEditorUtils.h"
#include "DataTableEditorUtils.h"
#include "Widgets/Views/SHeaderRow.h"

struct FMemoDataTable;

DECLARE_DELEGATE_OneParam(FOnRowHighlighted , FName /*Row name*/);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnSequencerNameChanged , FString);


/**
 * 
 */
class FMemoTableEditor : public IDataTableEditor , public FEditorUndoClient
	, public FStructureEditorUtils::INotifyOnStructChanged
	, public FDataTableEditorUtils::INotifyOnDataTableChanged 
{
	friend class SMemoTableListViewRow;

public:

	FMemoTableEditor();
	virtual ~FMemoTableEditor();

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	//// FEditorUndoClient
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	void HandleUndoRedo();

	//// INotifyOnStructChanged
	virtual void PreChange(const class UUserDefinedStruct* Struct , FStructureEditorUtils::EStructureEditorChangeInfo Info) override;
	virtual void PostChange(const class UUserDefinedStruct* Struct , FStructureEditorUtils::EStructureEditorChangeInfo Info) override;

	//// INotifyOnDataTableChanged
	virtual void PreChange(const UDataTable* Changed , FDataTableEditorUtils::EDataTableChangeInfo Info) override;
	virtual void PostChange(const UDataTable* Changed , FDataTableEditorUtils::EDataTableChangeInfo Info) override;
	virtual void SelectionChange(const UDataTable* Changed , FName RowName) override;


	FText GetCellText(TSharedPtr<FMemoDataTable> InRowDataPointer , int32 ColumnIndex) const;

	FText GetFilterText() const;
	FSlateColor GetRowTextColor(FName RowName) const;

	void OnFilterTextChanged(const FText& InFilterText);
	void OnFilterTextCommitted(const FText& NewText , ETextCommit::Type CommitInfo);

	/** Get the data table being edited */
	const UDataTable* GetDataTable() const;

	/** Struct holding information about the current column widths */
	struct FColumnWidth
	{
		FColumnWidth()
			: bIsAutoSized(true)
			, CurrentWidth(0.0f)
		{
		}

		/** True if this column is being auto-sized rather than sized by the user */
		bool bIsAutoSized;
		/** The width of the column, either sized by the user, or auto-sized */
		float CurrentWidth;
	};


	UDataTable* selectedDataTable;

	void SetHighlightedRow(FName Name);

	/** UI for the "Row Editor" tab */
	TSharedPtr<SSearchBox> SearchBoxWidget;

	/** UI for the "Data Table" tab */
	TSharedPtr<SWidget> DataTableTabWidget;

	/**	Spawns the tab with the data table inside */
	TSharedRef<SDockTab> SpawnTab_DataTable(const FSpawnTabArgs& Args);

	/** Helper function for creating and registering the tab containing the data table data */
	virtual void CreateAndRegisterDataTableTab(const TSharedRef<class FTabManager>& InTabManager);

	TSharedRef<SVerticalBox> CreateContentBox();

	/** Make the widget for a row entry in the data table row list view */
	TSharedRef<ITableRow> MakeRowWidget(TSharedPtr<FMemoDataTable> InRowDataPtr , const TSharedRef<STableViewBase>& OwnerTable);

	void OnRowSelectionChanged(TSharedPtr<FMemoDataTable> InNewSelection , ESelectInfo::Type InSelectInfo);

	void RefreshCachedDataTable(const FName InCachedSelection = NAME_None , const bool bUpdateEvenIfValid = false);

	void UpdateVisibleRows(const FName InCachedSelection = NAME_None , const bool bUpdateEvenIfValid = false);

	void RestoreCachedSelection(const FName InCachedSelection , const bool bUpdateEvenIfValid = false);

	/** Generic version that works with any datatable-like structure */
	static void GetDataForEditing(const UScriptStruct* RowStruct , const TMap<FName , uint8*>& RowMap , TArray<TSharedPtr<FMemoDataTableColumn>>& OutAvailableColumns , TArray<TSharedPtr<FMemoDataTable>>& OutAvailableRows);

	/** Array of the rows that match the active filter(s) */
	TArray<TSharedPtr<FMemoDataTable>> VisibleRows;

	/** Header row containing entries for each column in AvailableColumns */
	TSharedPtr<SHeaderRow> ColumnNamesHeaderRow;

	/** List view responsible for showing the rows in VisibleRows for each entry in AvailableColumns */
	TSharedPtr<SListView<TSharedPtr<FMemoDataTable>>> CellsListView;

	/** Array of the columns that are available for editing */
	TArray<TSharedPtr<FMemoDataTableColumn>> AvailableColumns;

	/** Array of the rows that are available for editing */
	TArray<TSharedPtr<FMemoDataTable>> AvailableRows;

	/** Widths of data table cell columns */
	TArray<FColumnWidth> ColumnWidths;

	/** The visible row index of the currently selected row */
	int32 HighlightedVisibleRowIndex;

	/** The current filter text applied to the data table */
	FText ActiveFilterText;

	/**	The tab id for the data table tab */
	FName DataTableTabId = FName("memo");

	/** The column id for the row name list view column */
	const FName RowNameColumnId = FName("RowNameColumnId");

	/** The column id for the row number list view column */
	const FName RowNumberColumnId = FName("RowNumberColumnId");

	/** The column id for the drag drop column */
	 const FName RowDragDropColumnId = FName("RowDragDropColumnId");

	 /** The name of the currently selected row */
	 FName HighlightedRowName;

	 FOnRowHighlighted CallbackOnRowHighlighted;

	 EColumnSortMode::Type GetColumnSortMode(const FName ColumnId) const;

	 void OnColumnSortModeChanged(const EColumnSortPriority::Type SortPriority , const FName& ColumnId , const EColumnSortMode::Type InSortMode);

	 void OnColumnNameSortModeChanged(const EColumnSortPriority::Type SortPriority , const FName& ColumnId , const EColumnSortMode::Type InSortMode);

	 TMap<FName , bool> GetSolvedComment(FName RowName) const;

	 bool isSolved;

	 /** Specify which column to sort with */
	 FName SortByColumn;

	 /** Currently selected sorting mode */
	 EColumnSortMode::Type SortMode;

	 FString comboBoxDefaultName = "All Seqeunce";


public:
	TArray<TSharedPtr<FString>> Options;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBoxWidget;

	void GetSequenceAsset();

	TSharedPtr<STextBlock> contentTitle;

	//FOnSequencerNameChanged sequnencerNameChanged;

	FReply OnDetailClicked();

	FReply OnWriteClicked();

	FReply RenewMemoContent();

	TSet<FString> ExtractUUIDs(const FString& FilePath);

	void CompareCSVFiles(const FString& File1Path , const FString& File2Path);

};
