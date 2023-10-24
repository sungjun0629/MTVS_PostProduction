// Fill out your copyright notice in the Description page of Project Settings.


#include "DragNDropWidget.h"
#include "EditorWidgets/Public/SDropTarget.h"

void DragNDropWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SDropTarget)
				.OnAllowDrop_Raw(this, &DragNDropWidget::OnAllowDrop)
		];
}

bool DragNDropWidget::OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation)
{
	return true;
}

FReply DragNDropWidget::OnDropVideo(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Dropped file path: "));
	return FReply::Handled();

}
