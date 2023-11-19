// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MemoDataTable.h"
#include "DataTablePractice.h"
#include "MemoTableEditor.h"
#include "Widgets/Views/STableRow.h"
#include "Templates/SharedPointer.h"

class FMemoTableEditor;
struct FMemoDataTable;



/**
 * A widget to represent a row in a Data Table Editor widget. This widget allows us to do things like right-click
 * and take actions on a particular row of a Data Table.
 */
class SMemoTableListViewRow : public SMultiColumnTableRow<TSharedPtr<FMemoDataTable>>
{
public:

	SLATE_BEGIN_ARGS(SMemoTableListViewRow)
		: _IsEditable(true)
		{
		}
	/** The owning object. This allows us access to the actual data table being edited as well as some other API functions. */
	SLATE_ARGUMENT(TSharedPtr<FMemoTableEditor> , DataTableEditor)
		/** The row we're working with to allow us to get naming information. */
		SLATE_ARGUMENT(TSharedPtr<FMemoDataTable> , RowDataPtr)
		SLATE_ARGUMENT(bool , IsEditable)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs , const TSharedRef<STableViewBase>& InOwnerTableView);

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry , const FPointerEvent& MouseEvent) override;

	virtual FReply OnKeyDown(const FGeometry& MyGeometry , const FKeyEvent& InKeyEvent) override;

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	virtual FReply 
	OnMouseButtonDoubleClick(const FGeometry& InMyGeometry , const FPointerEvent& InMouseEvent);

private:

	TSharedPtr<FName> CurrentName;

	TSharedPtr<FMemoDataTable> RowDataPtr;
	TWeakPtr<FMemoTableEditor> DataTableEditor;

	virtual const FSlateBrush* GetBorder() const;

	bool IsEditable;
};
