// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "MemoDataTable.h"
#include "MemoCommentTable.h"
#include "SMemoTableListViewRow.h"
#include "Widgets/Input/SCheckBox.h"

/**
 * 
 */
class SCommentDetail : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SCommentDetail) {}

	SLATE_END_ARGS()

public:
	~SCommentDetail();

	TSharedPtr<STextBlock> sequenceName;
	TSharedPtr<STextBlock> title;
	TSharedPtr<STextBlock> content;
	TSharedPtr<SCheckBox> isSolvedCheckbox;

	FString comment; 
	FString contentUUID;
	bool isSolved;

	TArray<FMemoDataTable*> TableRows;
	TSharedPtr<SListView<TSharedPtr<FMemoCommentTable>>> commentListView;
	TArray<TSharedPtr<FMemoCommentTable>> commentItems;
	TSharedPtr<SEditableText> commentInputBlank;
	TSharedPtr<SHeaderRow> ColumnNamesHeaderRow;

	void Construct(const FArguments& InArgs);

	void LoadContent(FString UUID);

	void ReloadListview();

	void ReloadCommentDetail(FString contentUUID);

	void OnCheckBoxClicked(ECheckBoxState state);

	FReply OnSubmitClicked();

	FReply OnDeleteClicked();


};
