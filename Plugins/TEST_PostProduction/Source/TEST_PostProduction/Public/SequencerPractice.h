// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "WebBrowser/Public/SWebBrowserView.h"
#include "MemoDataTable.h"

#define LOCTEXT_NAMESPACE "DataTableEditorPractice"


/**
 *
 */
class SSequencePractice : public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SSequencePractice) {}

	SLATE_END_ARGS()

public:

	FString title;
	FString sequenceName;
	FString content;
	FString comboBoxContent = "sequencer";
	FString SelectedItem;


	FString filterSequenceName = "";
	TArray<FMemoDataTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.


	TArray<TSharedPtr<FString>> Options;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBoxWidget;

	TSharedPtr<STextBlock> contentTitle;
	TArray<TSharedPtr<FString>> Items;
	TArray<TSharedPtr<FMemoDataTable>> memoItems;
	TSharedPtr<SListView<TSharedPtr<FMemoDataTable>>> csvListView;
	/** UI for the "Row Editor" tab */
	TSharedPtr<SSearchBox> SearchBoxWidget;

	/** Header row containing entries for each column in AvailableColumns */
	TSharedPtr<SHeaderRow> ColumnNamesHeaderRow;

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void GetSequenceAsset();

	void ChangeContent(FString content);

	void OnMousebuttonDoubleClick(TSharedPtr<FMemoDataTable> Item);

	FReply OnSubmitClicked();

	FReply OnDetailClicked();

	FReply OnWriteClicked();

	void OnColumnNumberSortModeChanged(const EColumnSortPriority::Type SortPriority , const FName& ColumnId , const EColumnSortMode::Type InSortMode);
};
