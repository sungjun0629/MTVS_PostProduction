// Fill out your copyright notice in the Description page of Project Settings.


#include "SDataTableListViewRowPractice.h"

#include "AssetRegistry/AssetData.h"
#include "Containers/Map.h"
//#include "DataTableEditor.h"
#include "DataTableUtils.h"
#include "Delegates/Delegate.h"
#include "Editor.h"
#include "Engine/DataTable.h"
#include "Framework/Application/MenuStack.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "HAL/PlatformCrt.h"
#include "HAL/PlatformMisc.h"
#include "Input/Events.h"
#include "Internationalization/Internationalization.h"
#include "Layout/Children.h"
#include "Layout/Margin.h"
#include "Layout/WidgetPath.h"
#include "Math/UnrealMathSSE.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Attribute.h"
#include "Misc/MessageDialog.h"
#include "SlotBase.h"
#include "Styling/AppStyle.h"
#include "Styling/CoreStyle.h"
#include "Styling/ISlateStyle.h"
#include "Templates/Casts.h"
#include "Textures/SlateIcon.h"
#include "UObject/UnrealNames.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SHeaderRow.h"
#include "DataTablePractice.h"

class STableViewBase;
class SWidget;
struct FGeometry;
struct FSlateBrush;

#define LOCTEXT_NAMESPACE "SDataTableListViewRowName"

void SDataTableListViewRowPractice::Construct(const FArguments& InArgs , const TSharedRef<STableViewBase>& InOwnerTableView)
{
	RowDataPtr = InArgs._RowDataPtr;
	CurrentName = MakeShareable(new FName(RowDataPtr->RowId));
	DataTableEditor = InArgs._DataTableEditor;
	IsEditable = InArgs._IsEditable;
	SMultiColumnTableRow<FDataTableEditorRowListViewDataPtr>::Construct(
		FSuperRowType::FArguments()
		.Style(FAppStyle::Get() , "DataTableEditor.CellListViewRow")
		.OnDrop(this , &SDataTableListViewRowPractice::OnRowDrop)
		.OnDragEnter(this , &SDataTableListViewRowPractice::OnRowDragEnter)
		.OnDragLeave(this , &SDataTableListViewRowPractice::OnRowDragLeave) ,
		InOwnerTableView
	);

	SetBorderImage(TAttribute<const FSlateBrush*>(this , &SDataTableListViewRowPractice::GetBorder));

}

FReply SDataTableListViewRowPractice::OnMouseButtonUp(const FGeometry& MyGeometry , const FPointerEvent& MouseEvent)
{
	if ( IsEditable && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton && RowDataPtr.IsValid() && FEditorDelegates::OnOpenReferenceViewer.IsBound() && DataTableEditor.IsValid() )
	{
		FDataTableEditorUtils::SelectRow(DataTableEditor.Pin()->GetDataTable() , RowDataPtr->RowId);

		TSharedRef<SWidget> MenuWidget = MakeRowActionsMenu();

		FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
		FSlateApplication::Get().PushMenu(AsShared() , WidgetPath , MenuWidget , MouseEvent.GetScreenSpacePosition() , FPopupTransitionEffect::ContextMenu);
		return FReply::Handled();
	}

	return STableRow::OnMouseButtonUp(MyGeometry , MouseEvent);
}

void SDataTableListViewRowPractice::OnSearchForReferences()
{
	if ( DataTableEditor.IsValid() && RowDataPtr.IsValid() )
	{
		if ( FDataTablePractice* DataTableEditorPtr = DataTableEditor.Pin().Get() )
		{
			UDataTable* SourceDataTable = const_cast< UDataTable* >( DataTableEditorPtr->GetDataTable() );

			TArray<FAssetIdentifier> AssetIdentifiers;
			AssetIdentifiers.Add(FAssetIdentifier(SourceDataTable , RowDataPtr->RowId));

			FEditorDelegates::OnOpenReferenceViewer.Broadcast(AssetIdentifiers , FReferenceViewerParams());
		}
	}
}

void SDataTableListViewRowPractice::OnInsertNewRow(ERowInsertionPosition InsertPosition)
{
	if ( DataTableEditor.IsValid() && RowDataPtr.IsValid() )
	{
		if ( FDataTablePractice* DataTableEditorPtr = DataTableEditor.Pin().Get() )
		{
			UDataTable* SourceDataTable = const_cast< UDataTable* >( DataTableEditorPtr->GetDataTable() );

			if ( SourceDataTable )
			{
				FName NewName = DataTableUtils::MakeValidName(TEXT("NewRow"));
				while ( SourceDataTable->GetRowMap().Contains(NewName) )
				{
					NewName.SetNumber(NewName.GetNumber() + 1);
				}

				if ( InsertPosition == ERowInsertionPosition::Bottom )
				{
					FDataTableEditorUtils::AddRow(SourceDataTable , NewName);
				}
				else
				{
					FDataTableEditorUtils::AddRowAboveOrBelowSelection(SourceDataTable , *CurrentName , NewName , InsertPosition);
				}
				FDataTableEditorUtils::SelectRow(SourceDataTable , NewName);

				DataTableEditorPtr->SetDefaultSort();
			}
		}
	}
}

FReply SDataTableListViewRowPractice::OnRowDrop(const FDragDropEvent& DragDropEvent)
{
	bIsHoveredDragTarget = false;

	TSharedPtr<FDataTableRowDragDropOpPractice> DataTableDropOp = DragDropEvent.GetOperationAs< FDataTableRowDragDropOpPractice >();
	TSharedPtr<SDataTableListViewRowPractice> RowPtr = nullptr;
	if ( DataTableDropOp.IsValid() && DataTableDropOp->Row.IsValid() )
	{
		RowPtr = DataTableDropOp->Row.Pin();
	}
	if ( !RowPtr.IsValid() )
	{
		return FReply::Unhandled();
	}

	int32 JumpCount = ( RowPtr->RowDataPtr )->RowNum - RowDataPtr->RowNum;

	if ( !JumpCount )
	{
		return FReply::Handled();
	}

	FDataTableEditorUtils::ERowMoveDirection Direction = JumpCount > 0 ? FDataTableEditorUtils::ERowMoveDirection::Up : FDataTableEditorUtils::ERowMoveDirection::Down;

	if ( FDataTablePractice* DataTableEditorPtr = DataTableEditor.Pin().Get() )
	{
		UDataTable* SourceDataTable = const_cast< UDataTable* >( DataTableEditorPtr->GetDataTable() );

		if ( SourceDataTable )
		{
			FName& RowId = ( RowPtr->RowDataPtr )->RowId;

			FDataTableEditorUtils::MoveRow(SourceDataTable , RowId , Direction , FMath::Abs<int32>(JumpCount));

			FDataTableEditorUtils::SelectRow(SourceDataTable , RowId);

			DataTableEditorPtr->SortMode = EColumnSortMode::Ascending;
			DataTableEditorPtr->SortByColumn = FDataTablePractice::RowNumberColumnId;

			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

FReply SDataTableListViewRowPractice::OnKeyDown(const FGeometry& MyGeometry , const FKeyEvent& InKeyEvent)
{
	FKey Key = InKeyEvent.GetKey();

	if ( Key == EKeys::Escape && InlineEditableText->HasKeyboardFocus() )
	{
		// Clear focus
		return FReply::Handled().SetUserFocus(SharedThis(this) , EFocusCause::Cleared);
	}

	return FReply::Unhandled();
}

void SDataTableListViewRowPractice::OnRowRenamed(const FText& Text , ETextCommit::Type CommitType)
{
	UDataTable* DataTable = Cast<UDataTable>(DataTableEditor.Pin()->GetEditingObject());

	if ( !GetCurrentNameAsText().EqualTo(Text) && DataTable )
	{
		if ( FDataTablePractice* DataTableEditorPtr = DataTableEditor.Pin().Get() )
		{

			const TArray<FName> RowNames = DataTable->GetRowNames();

			if ( Text.IsEmptyOrWhitespace() || !FName::IsValidXName(Text.ToString() , INVALID_NAME_CHARACTERS) )
			{
				// Only pop up the error dialog if the rename was caused by the user's action
				if ( ( CommitType == ETextCommit::OnEnter ) || ( CommitType == ETextCommit::OnUserMovedFocus ) )
				{
					// popup an error dialog here
					const FText Message = FText::Format(LOCTEXT("InvalidRowName" , "'{0}' is not a valid row name") , Text);
					FMessageDialog::Open(EAppMsgType::Ok , Message);
				}
				return;
			}
			const FName NewName = DataTableUtils::MakeValidName(Text.ToString());
			if ( NewName == NAME_None )
			{
				// popup an error dialog here
				const FText Message = FText::Format(LOCTEXT("InvalidRowName" , "'{0}' is not a valid row name") , Text);
				FMessageDialog::Open(EAppMsgType::Ok , Message);

				return;
			}
			for ( const FName& Name : RowNames )
			{
				if ( Name.IsValid() && ( Name == NewName ) )
				{
					//the name already exists
					// popup an error dialog here
					const FText Message = FText::Format(LOCTEXT("DuplicateRowName" , "'{0}' is already used as a row name in this table") , Text);
					FMessageDialog::Open(EAppMsgType::Ok , Message);
					return;

				}
			}

			const FName OldName = GetCurrentName();
			FDataTableEditorUtils::RenameRow(DataTable , OldName , NewName);
			FDataTableEditorUtils::SelectRow(DataTable , NewName);

			DataTableEditorPtr->SetDefaultSort();

			*CurrentName = NewName;
		}
	}
}

TSharedRef<SWidget> SDataTableListViewRowPractice::GenerateWidgetForColumn(const FName& ColumnName)
{
	TSharedPtr<FDataTablePractice> DataTableEditorPtr = DataTableEditor.Pin();
	return ( DataTableEditorPtr.IsValid() )
		? MakeCellWidget(IndexInList , ColumnName)
		: SNullWidget::NullWidget;
}

TSharedRef<SWidget> SDataTableListViewRowPractice::MakeCellWidget(const int32 InRowIndex , const FName& InColumnId)
{
	const FName RowDragDropColumnId("RowDragDrop");

	int32 ColumnIndex = 0;

	FDataTablePractice* DataTableEdit = DataTableEditor.Pin().Get();
	TArray<FDataTableEditorColumnHeaderDataPtr>& AvailableColumns = DataTableEdit->AvailableColumns;

	if ( InColumnId.IsEqual(RowDragDropColumnId) )
	{
		return SNew(SDataTableRowHandlePractice)
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
					.ColorAndOpacity(DataTableEdit , &FDataTablePractice::GetRowTextColor , RowDataPtr->RowId)
					.HighlightText(DataTableEdit , &FDataTablePractice::GetFilterText)
			];
	}

	const FName RowNameColumnId("RowName");

	if ( InColumnId.IsEqual(RowNameColumnId) )
	{
		return SNew(SBox)
			.Padding(FMargin(4 , 2 , 4 , 2))
			[
				SAssignNew(InlineEditableText , SInlineEditableTextBlock)
					.Text(RowDataPtr->DisplayName)
					.OnTextCommitted(this , &SDataTableListViewRowPractice::OnRowRenamed)
					.HighlightText(DataTableEdit , &FDataTablePractice::GetFilterText)
					.ColorAndOpacity(DataTableEdit , &FDataTablePractice::GetRowTextColor , RowDataPtr->RowId)
					.IsReadOnly(!IsEditable)
			];
	}

	for ( ; ColumnIndex < AvailableColumns.Num(); ++ColumnIndex )
	{
		const FDataTableEditorColumnHeaderDataPtr& ColumnData = AvailableColumns[ ColumnIndex ];
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
					.ColorAndOpacity(DataTableEdit , &FDataTablePractice::GetRowTextColor , RowDataPtr->RowId)
					.Text(DataTableEdit , &FDataTablePractice::GetCellText , RowDataPtr , ColumnIndex)
					.HighlightText(DataTableEdit , &FDataTablePractice::GetFilterText)
					.ToolTipText(DataTableEdit , &FDataTablePractice::GetCellToolTipText , RowDataPtr , ColumnIndex)
			];
	}

	return SNullWidget::NullWidget;
}

FName SDataTableListViewRowPractice::GetCurrentName() const
{
	return CurrentName.IsValid() ? *CurrentName : NAME_None;

}

uint32 SDataTableListViewRowPractice::GetCurrentIndex() const
{
	return RowDataPtr.IsValid() ? RowDataPtr->RowNum : -1;
}

const TArray<FText>& SDataTableListViewRowPractice::GetCellValues() const
{
	check(RowDataPtr)
		return RowDataPtr->CellData;
}

FReply SDataTableListViewRowPractice::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry , const FPointerEvent& InMouseEvent)
{
	if ( InlineEditableText->IsHovered() )
	{
		InlineEditableText->EnterEditingMode();
	}

	return FReply::Handled();
}

void SDataTableListViewRowPractice::SetRowForRename()
{
	InlineEditableText->EnterEditingMode();
}

const FDataTableEditorRowListViewDataPtr& SDataTableListViewRowPractice::GetRowDataPtr() const
{
	return RowDataPtr;
}

FText SDataTableListViewRowPractice::GetCurrentNameAsText() const
{
	return FText::FromName(GetCurrentName());
}

void SDataTableRowHandlePractice::Construct(const FArguments& InArgs)
{
	ParentRow = InArgs._ParentRow;

	ChildSlot
		[
			InArgs._Content.Widget
		];
}

FReply SDataTableRowHandlePractice::OnDragDetected(const FGeometry& MyGeometry , const FPointerEvent& MouseEvent)
{
	if ( MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) )
	{
		TSharedPtr<FDragDropOperation> DragDropOp = CreateDragDropOperation(ParentRow.Pin());
		if ( DragDropOp.IsValid() )
		{
			return FReply::Handled().BeginDragDrop(DragDropOp.ToSharedRef());
		}
	}

	return FReply::Unhandled();

}

TSharedPtr<FDataTableRowDragDropOpPractice> SDataTableRowHandlePractice::CreateDragDropOperation(TSharedPtr<SDataTableListViewRowPractice> InRow)
{
	TSharedPtr<FDataTableRowDragDropOpPractice> Operation = MakeShareable(new FDataTableRowDragDropOpPractice(InRow));

	return Operation;
}

void SDataTableListViewRowPractice::SetIsDragDrop(bool bInIsDragDrop)
{
	bIsDragDropObject = bInIsDragDrop;
}


void SDataTableListViewRowPractice::OnRowDragEnter(const FDragDropEvent& DragDropEvent)
{
	bIsHoveredDragTarget = true;
}

void SDataTableListViewRowPractice::OnRowDragLeave(const FDragDropEvent& DragDropEvent)
{
	bIsHoveredDragTarget = false;
}


const FSlateBrush* SDataTableListViewRowPractice::GetBorder() const
{
	if ( bIsDragDropObject )
	{
		return FAppStyle::GetBrush("DataTableEditor.DragDropObject");
	}
	else if ( bIsHoveredDragTarget )
	{
		return FAppStyle::GetBrush("DataTableEditor.DragDropHoveredTarget");
	}
	else
	{
		return STableRow::GetBorder();
	}
}

void SDataTableListViewRowPractice::OnMoveToExtentClicked(FDataTableEditorUtils::ERowMoveDirection MoveDirection)
{
	if ( DataTableEditor.IsValid() )
	{
		TSharedPtr<FDataTablePractice> DataTableEditorPtr = DataTableEditor.Pin();
		DataTableEditorPtr->OnMoveToExtentClicked(MoveDirection);
	}
}


TSharedRef<SWidget> SDataTableListViewRowPractice::MakeRowActionsMenu()
{
	FMenuBuilder MenuBuilder(true , DataTableEditor.Pin()->GetToolkitCommands());

	MenuBuilder.AddMenuEntry(
		LOCTEXT("DataTableRowMenuActions_InsertNewRow" , "Insert New Row") ,
		LOCTEXT("DataTableRowMenuActions_InsertNewRowTooltip" , "Insert a new row") ,
		FSlateIcon(FAppStyle::GetAppStyleSetName() , "DataTableEditor.Add") ,
		FUIAction(FExecuteAction::CreateSP(this , &SDataTableListViewRowPractice::OnInsertNewRow , ERowInsertionPosition::Bottom))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("DataTableRowMenuActions_InsertNewRowAbove" , "Insert New Row Above") ,
		LOCTEXT("DataTableRowMenuActions_InsertNewRowAboveTooltip" , "Insert a new Row above the current selection") ,
		FSlateIcon() ,
		FUIAction(FExecuteAction::CreateSP(this , &SDataTableListViewRowPractice::OnInsertNewRow , ERowInsertionPosition::Above))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("DataTableRowMenuActions_InsertNewRowBelow" , "Insert New Row Below") ,
		LOCTEXT("DataTableRowMenuActions_InsertNewRowBelowTooltip" , "Insert a new Row below the current selection") ,
		FSlateIcon() ,
		FUIAction(FExecuteAction::CreateSP(this , &SDataTableListViewRowPractice::OnInsertNewRow , ERowInsertionPosition::Below))
	);

	MenuBuilder.AddMenuEntry(FGenericCommands::Get().Copy);
	MenuBuilder.AddMenuEntry(FGenericCommands::Get().Paste);
	MenuBuilder.AddMenuEntry(FGenericCommands::Get().Duplicate);
	MenuBuilder.AddMenuEntry(FGenericCommands::Get().Rename);
	MenuBuilder.AddMenuEntry(FGenericCommands::Get().Delete);

	MenuBuilder.AddMenuSeparator();

	MenuBuilder.AddMenuEntry(
		LOCTEXT("DataTableRowMenuActions_MoveToTopAction" , "Move Row to Top") ,
		LOCTEXT("DataTableRowMenuActions_MoveToTopActionTooltip" , "Move selected Row to the top") ,
		FSlateIcon(FAppStyle::GetAppStyleSetName() , "Symbols.DoubleUpArrow") ,
		FUIAction(FExecuteAction::CreateSP(this , &SDataTableListViewRowPractice::OnMoveToExtentClicked , FDataTableEditorUtils::ERowMoveDirection::Up))
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("DataTableRowMenuActions_MoveToBottom" , "Move Row To Bottom") ,
		LOCTEXT("DataTableRowMenuActions_MoveToBottomTooltip" , "Move selected Row to the bottom") ,
		FSlateIcon(FAppStyle::GetAppStyleSetName() , "Symbols.DoubleDownArrow") ,
		FUIAction(FExecuteAction::CreateSP(this , &SDataTableListViewRowPractice::OnMoveToExtentClicked , FDataTableEditorUtils::ERowMoveDirection::Down))
	);

	MenuBuilder.AddMenuSeparator();

	MenuBuilder.AddMenuEntry(
		NSLOCTEXT("FDataTableRowUtils" , "FDataTableRowUtils_SearchForReferences" , "Find Row References") ,
		NSLOCTEXT("FDataTableRowUtils" , "FDataTableRowUtils_SearchForReferencesTooltip" , "Find assets that reference this Row") ,
		FSlateIcon() ,
		FUIAction(FExecuteAction::CreateSP(this , &SDataTableListViewRowPractice::OnSearchForReferences))
	);

	return MenuBuilder.MakeWidget();
}

FDataTableRowDragDropOpPractice::FDataTableRowDragDropOpPractice(TSharedPtr<SDataTableListViewRowPractice> InRow)
{
	Row = InRow;

	TSharedPtr<SDataTableListViewRowPractice> RowPtr = nullptr;
	if ( Row.IsValid() )
	{
		RowPtr = Row.Pin();
		RowPtr->SetIsDragDrop(true);

		DecoratorWidget = SNew(SBorder)
			.Padding(8.f)
			.BorderImage(FAppStyle::GetBrush("Graph.ConnectorFeedback.Border"))
			.Content()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
							.Text(FText::Format(NSLOCTEXT("DataTableDragDrop" , "PlaceRowHere" , "Place Row {0} Here") , FText::AsNumber(InRow->GetCurrentIndex())))
					]
			];

		Construct();
	}
}

void FDataTableRowDragDropOpPractice::OnDrop(bool bDropWasHandled , const FPointerEvent& MouseEvent)
{
	FDecoratedDragDropOp::OnDrop(bDropWasHandled , MouseEvent);

	TSharedPtr<SDataTableListViewRowPractice> RowPtr = nullptr;
	if ( Row.IsValid() )
	{
		RowPtr = Row.Pin();
		RowPtr->SetIsDragDrop(false);
	}
}

#undef LOCTEXT_NAMESPACE
