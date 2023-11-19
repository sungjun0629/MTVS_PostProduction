// Fill out your copyright notice in the Description page of Project Settings.


#include "SMemoTableListViewRow.h"
#include "Widgets/SWidget.h"

void SMemoTableListViewRow::Construct(const FArguments& InArgs , const TSharedRef<STableViewBase>& InOwnerTableView)
{
	/*RowDataPtr = InArgs._RowDataPtr;
	CurrentName = MakeShareable(new FName(RowDataPtr->sequenceName));
	DataTableEditor = InArgs._DataTableEditor;	
	IsEditable = InArgs._IsEditable;*/

	SMultiColumnTableRow<TSharedPtr<FMemoDataTable>>::Construct(
		FSuperRowType::FArguments()
		.Style(FAppStyle::Get() , "DataTableEditor.CellListViewRow")
		, InOwnerTableView
	);

	SetBorderImage(TAttribute<const FSlateBrush*>(this , &SMemoTableListViewRow::GetBorder));
}

FReply SMemoTableListViewRow::OnMouseButtonUp(const FGeometry& MyGeometry , const FPointerEvent& MouseEvent)
{

	return FReply::Handled();
}

FReply SMemoTableListViewRow::OnKeyDown(const FGeometry& MyGeometry , const FKeyEvent& InKeyEvent)
{

	return FReply::Handled();
}

TSharedRef<SWidget> SMemoTableListViewRow::GenerateWidgetForColumn(const FName& ColumnName)
{

	return SNew(SVerticalBox);
}

FReply SMemoTableListViewRow::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry , const FPointerEvent& InMouseEvent)
{

	return FReply::Handled();
}

const FSlateBrush* SMemoTableListViewRow::GetBorder() const
{
	return STableRow::GetBorder();
}
