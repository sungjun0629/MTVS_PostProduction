// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SWriteContent : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SWriteContent) {}

	SLATE_END_ARGS()

public:
	FString title;
	FString sequenceName;
	FString content;

public:
	void Construct(const FArguments& InArgs);

	FReply OnSubmitClicked();
};
