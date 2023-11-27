// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "MemoDetailTable.h"
/**
 * 
 */

class SSequencerDetail : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SSequencerDetail) {}

	SLATE_END_ARGS()

public:
	~SSequencerDetail();

	TSharedPtr<SImage> image;
	TSharedPtr<SEditableText> title;
	TSharedPtr<STextBlock> participants;
	TSharedPtr<STextBlock> buttonText;
	TSharedPtr<SButton> imageButton;
	TSharedPtr<SEditableText> content;

	TSharedPtr<SEditableText> sceneInfo;
	TArray<FMemoDetailTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.

	FString detailSequenceName;
	bool isEditable = false;
	FString imageRePath;

public:
	void Construct(const FArguments& InArgs);

	void ReloadContent(FString sequenceName);

	FString GetImagePath(FString sequenceName);

	void ReloadDetailPage(FString sequenceName);

	void OnTitleCommited(const FText& Intext, ETextCommit::Type type);
	void OnContentCommited(const FText& Intext, ETextCommit::Type type);
	void OnInfoCommited(const FText& Intext, ETextCommit::Type type);

	FReply OnModifyButtonClicked();
	FReply OnimageButtonClicked();
};
