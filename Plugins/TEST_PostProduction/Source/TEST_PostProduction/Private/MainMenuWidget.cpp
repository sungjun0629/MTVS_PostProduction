// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "EditorWidgets/Public/SDropTarget.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "DragAndDrop/ActorDragDropOp.h"
#include "Input/DragAndDrop.h"
#include "FileToBase64Uploader_Plugin.h"
//#include "../../../../AAPostProduction/MTVS_PostProduction/Source/PostProduction/Public/FileToBase64Uploader.h"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{

	ChildSlot
	[
		SNew(SDropTarget)
		.OnAllowDrop_Raw(this, &SMainMenuWidget::OnAllowDrop)
		.OnDropped(this, &SMainMenuWidget::OnDropVideo)
		[
			SNew(SVerticalBox)
				
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 400, 0, 0)
				[
					SNew(SButton)
						.VAlign(VAlign_Center)
						.Text(FText::FromString("Upload File"))
						.OnClicked(this, &SMainMenuWidget::OnUploadFileClicked)
						.HAlign(HAlign_Center)
				]
		]
	];
}

bool SMainMenuWidget::OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation)
{
	return true;
}

FReply SMainMenuWidget::OnDropVideo(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FDragDropOperation> DragDropOperation = DragDropEvent.GetOperation();

	if (DragDropOperation.IsValid() && DragDropOperation->IsOfType<FAssetDragDropOp>())
	{
		/*TSharedPtr<FAssetDragDropOp> FileOperation = StaticCastSharedPtr<FAssetDragDropOp>(DragDropOperation);
		const TArray<FString>& DroppedFiles = FileOperation->GetAssetPaths();*/

		// find Asset directory 
		/*for (const FAssetData& AssetData : FileOperation->GetAssets())
		{
			UE_LOG(LogTemp, Warning, TEXT("Dropped file path: %s"), *AssetData.GetObjectPathString());
		}*/
		UE_LOG(LogTemp, Warning, TEXT("Can't Drop UASSET"));
	}
	else if(DragDropOperation.IsValid() && DragDropOperation->IsOfType<FExternalDragOperation>())
	{
		TSharedPtr<FExternalDragOperation> FileOperation = StaticCastSharedPtr<FExternalDragOperation>(DragDropOperation);
		UE_LOG(LogTemp, Warning, TEXT("Dropped file path num: %s"), **FileOperation->GetFiles().begin());

		SetAssetPath(*FileOperation->GetFiles().begin());
		
		return FReply::Handled();
	}

	return FReply::Unhandled();

}

FReply SMainMenuWidget::OnUploadFileClicked()
{
	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FileUpload->UploadFile(AssetPath);
	return FReply::Handled();
}

