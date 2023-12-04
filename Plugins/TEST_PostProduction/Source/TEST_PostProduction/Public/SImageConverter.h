// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "SGetWebAddress.h"

/**
 * 
 */
class SImageConverter : public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SImageConverter) {}

	SLATE_END_ARGS()

public:
	TSharedPtr<STextBlock> dropText;

	FString description;
	
	FOnDownloadProgress OnDownloadProgressDelegate;

	FOnFileToStorageDownloadComplete OnFileToStorageDownloadCompleteDelegate;

public:

	TSharedPtr<STextBlock> title;

	void Construct(const FArguments& InArgs);

	FString GetAssetPath() { return AssetPath; }

	void SetAssetPath(FString Path) { AssetPath = Path; }

	bool OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation);
	
	FReply OnDropImage(const FGeometry& MyGeometry , const FDragDropEvent& DragDropEvent);
	
	FReply OnVideoUploadFileClicked();

	void OnGet3DImage(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);


private:
	FString AssetPath;
};
