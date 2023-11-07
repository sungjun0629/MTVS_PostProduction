// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "WebBrowser/Public/SWebBrowserView.h"
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
	TArray<TSharedPtr<FString>> Options;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBoxWidget;
	TSharedPtr<STextBlock> contentTitle;

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void GetSequenceAsset();

	void ChangeContent(FString content);

	FReply OnSubmitClicked();

};
