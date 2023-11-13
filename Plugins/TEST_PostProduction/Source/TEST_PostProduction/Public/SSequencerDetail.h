// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
/**
 * 
 */

class SSequencerDetail : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SSequencerDetail) {}

	SLATE_END_ARGS()

public:
	TSharedPtr<SImage> image;
	TSharedPtr<STextBlock> title;
	TSharedPtr<STextBlock> period;
	TSharedPtr<STextBlock> author;

	TSharedPtr<STextBlock> sceneInfo;

public:
	void Construct(const FArguments& InArgs);

	void ReloadContent(FString _title , FString _period , FString _author , FString _sceneInfo);
};
