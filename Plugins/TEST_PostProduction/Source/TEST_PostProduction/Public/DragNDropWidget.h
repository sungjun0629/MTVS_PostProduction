// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class DragNDropWidget : public SCompoundWidget
{
	// to Pass Args 
	SLATE_BEGIN_ARGS(DragNDropWidget) {}

	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

	bool OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation);
	
	FReply OnDropVideo(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent);
};