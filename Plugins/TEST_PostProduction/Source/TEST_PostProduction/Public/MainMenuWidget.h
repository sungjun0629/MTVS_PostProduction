// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SMainMenuWidget : public SCompoundWidget
{
	// to Pass Args 
	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

	bool OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation);

	FString GetAssetPath() { return AssetPath; }

	void SetAssetPath(FString Path) { AssetPath = Path; }	
	
	FReply OnDropVideo(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent);

	FReply	OnUploadFileClicked();


private:
	FString AssetPath;
};