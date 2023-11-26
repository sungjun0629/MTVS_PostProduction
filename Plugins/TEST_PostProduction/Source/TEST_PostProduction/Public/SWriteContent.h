// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
static class SWriteContent : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SWriteContent) {}

	SLATE_END_ARGS()

public:
	~SWriteContent();
	FString title;
	FString sequenceName;
	FString content;
	int32 priority;

	TArray<TSharedPtr<FString>> Options;

	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBoxWidget;

	TSharedPtr<STextBlock> comboBoxContent;

public:
	void Construct(const FArguments& InArgs);

	void ReloadSequenceName(FString SequenceName);

	FReply OnSubmitClicked();
};
