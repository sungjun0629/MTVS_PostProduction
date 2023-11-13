// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


DECLARE_DELEGATE(FOnGetResponse);

class SMainMenuWidget : public SCompoundWidget
{
	// to Pass Args 
	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_END_ARGS()

public:
	
	TSharedPtr<STextBlock> dropText;

	TSharedPtr<STextBlock> dropVideoText;

	FOnGetResponse OnGetResponse;

	void Construct(const FArguments& InArgs);

	bool OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation);

	FString GetAssetPath() { return AssetPath; }

	void SetAssetPath(FString Path) { AssetPath = Path; }	
	
	FReply OnDropVideo(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent);
	
	FReply OnDropImage(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent);

	FReply	OnUploadFileClicked();

	FReply	OnVideoUploadFileClicked();

	// After Upload Video, Wait for Response and Send Event
	void OnGetMMDone(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully);

private:
	FString AssetPath;
};