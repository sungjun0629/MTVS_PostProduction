// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "MemoTableEditor.h"


class SPostProductionWidget : public SCompoundWidget
{
	// to Pass Args 
	SLATE_BEGIN_ARGS(SPostProductionWidget) {}
	
	//SLATE_ARGUMENT(FString, ID);
	//SLATE_ARGUMENT(FString, PW);

	SLATE_END_ARGS()

public:

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OnKakaoLoginClicked();

	FReply OnGitHubLoginClicked();

	//TSharedPtr<FMemoTableEditor> MemoTableEditorInstance = MakeShared<FMemoTableEditor>();
};