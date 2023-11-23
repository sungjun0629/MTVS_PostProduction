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
	TSharedPtr<STextBlock> title;
	TSharedPtr<STextBlock> participants;
	TSharedPtr<STextBlock> content;

	TSharedPtr<STextBlock> sceneInfo;
	TArray<FMemoDetailTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.



public:
	void Construct(const FArguments& InArgs);

	void ReloadContent(FString sequenceName);

	FString GetImagePath(FString sequenceName);

	void ReloadDetailPage(FString sequenceName);
};
