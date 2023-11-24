// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "MemoDataTable.h"
#include "MemoCommentTable.h"
#include "SMemoTableListViewRow.h"

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

	FString comment; 
	FString contentUUID;

	TArray<FMemoDataTable*> TableRows;
	TSharedPtr<SListView<TSharedPtr<FMemoCommentTable>>> commentListView;
	TArray<TSharedPtr<FMemoCommentTable>> commentItems;
	TSharedPtr<SEditableText> commentInputBlank;
	TSharedPtr<SHeaderRow> ColumnNamesHeaderRow;

	void Construct(const FArguments& InArgs);

	void LoadContent(FString UUID);

	void ReloadListview();

	void ReloadCommentDetail(FString contentUUID);

	FReply OnSubmitClicked();
};
