// Fill out your copyright notice in the Description page of Project Settings.


#include "SMemoTableListViewRow.h"
#include "Widgets/SWidget.h"
#include "DataTableEditor/Private/DataTableEditor.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "DataTableEditor/Private/SDataTableListViewRow.h"
#include "Widgets/Views/STableRow.h"

void SMemoTableListViewRow::Construct(const FArguments& InArgs , const TSharedRef<STableViewBase>& InOwnerTableView)
{

	UE_LOG(LogTemp,Warning,TEXT("SMemoTableListViewRow Constructor"))
	RowDataPtr = InArgs._RowDataPtr;
	CurrentName = MakeShareable(new FName(RowDataPtr->sequenceName));
	DataTableEditor = InArgs._DataTableEditor;	
	IsEditable = InArgs._IsEditable;

	SMultiColumnTableRow<TSharedPtr<FMemoDataTable>>::Construct(
		FSuperRowType::FArguments()
		.Style(FAppStyle::Get() , "DataTableEditor.CellListViewRow")
		, InOwnerTableView
	);

	SetBorderImage(TAttribute<const FSlateBrush*>(this , &SMemoTableListViewRow::GetBorder));
}

FReply SMemoTableListViewRow::OnMouseButtonUp(const FGeometry& MyGeometry , const FPointerEvent& MouseEvent)
{
	return STableRow::OnMouseButtonUp(MyGeometry , MouseEvent);
}

FReply SMemoTableListViewRow::OnKeyDown(const FGeometry& MyGeometry , const FKeyEvent& InKeyEvent)
{
	return FReply::Handled();
}

TSharedRef<SWidget> SMemoTableListViewRow::GenerateWidgetForColumn(const FName& ColumnName)
{

	TSharedPtr<FMemoTableEditor> DataTableEditorPtr = DataTableEditor.Pin();
	return ( DataTableEditorPtr.IsValid() )
		? MakeCellWidget(IndexInList , ColumnName)
		: SNullWidget::NullWidget;
	
}

FReply SMemoTableListViewRow::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry , const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp,Warning,TEXT("OnMouseButtonDoubleClick"))
	return FReply::Handled();
}

TSharedRef<SWidget> SMemoTableListViewRow::MakeCellWidget(const int32 InRowIndex , const FName& InColumnId)
{
	const FName RowDragDropColumnId("RowDragDrop");

	int32 ColumnIndex = 0;

	FMemoTableEditor* DataTableEdit = DataTableEditor.Pin().Get();
	TArray<TSharedPtr<FMemoDataTableColumn>>& AvailableColumns = DataTableEdit->AvailableColumns;

	/*if ( InColumnId.IsEqual(RowDragDropColumnId) )
	{
		return SNew(SDataTableRowHandle)
			.Content()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(5.0f , 1.0f)
					[
						SNew(SImage)
							.Image(FCoreStyle::Get().GetBrush("VerticalBoxDragIndicatorShort"))
					]
			]
			.ParentRow(SharedThis(this));
	}

	const FName RowNumberColumnId("RowNumber");

	if ( InColumnId.IsEqual(RowNumberColumnId) )
	{
		return SNew(SBox)
			.Padding(FMargin(4 , 2 , 4 , 2))
			[
				SNew(STextBlock)
					.TextStyle(FAppStyle::Get() , "DataTableEditor.CellText")
					.Text(FText::FromString(FString::FromInt(RowDataPtr->RowNum)))
					.ColorAndOpacity(DataTableEdit , &FDataTableEditor::GetRowTextColor , RowDataPtr->RowId)
					.HighlightText(DataTableEdit , &FDataTableEditor::GetFilterText)
			];
	}
	*/

	for ( ; ColumnIndex < AvailableColumns.Num(); ++ColumnIndex )
	{
		const TSharedPtr<FMemoDataTableColumn>& ColumnData = AvailableColumns[ ColumnIndex ];
		if ( ColumnData->ColumnId == InColumnId )
		{
			break;
		}
	}
	
	// Valid column ID?
	if ( AvailableColumns.IsValidIndex(ColumnIndex) && RowDataPtr->CellData.IsValidIndex(ColumnIndex) )
	{
		return SNew(SBox)
			.Padding(FMargin(4 , 2 , 4 , 2))
			[
				SNew(STextBlock)
					.TextStyle(FAppStyle::Get() , "DataTableEditor.CellText")
					.ColorAndOpacity(DataTableEdit , &FMemoTableEditor::GetRowTextColor , RowDataPtr->RowId)
					.Text(DataTableEdit , &FMemoTableEditor::GetCellText , RowDataPtr , ColumnIndex)
					.HighlightText(DataTableEdit , &FMemoTableEditor::GetFilterText)
					//.ToolTipText(DataTableEdit , &FMemoTableEditor::GetCellToolTipText , RowDataPtr , ColumnIndex)
			];
	}

	return SNullWidget::NullWidget;
}

const FSlateBrush* SMemoTableListViewRow::GetBorder() const
{
	return STableRow::GetBorder();
}
