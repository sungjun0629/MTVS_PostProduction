// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "EditorWidgets/Public/SDropTarget.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "DragAndDrop/ActorDragDropOp.h"
#include "Input/DragAndDrop.h"
#include "FileToBase64Uploader_Plugin.h"
#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "JsonParseLibrary_Plugin.h"
#include "MotionMenuWidget.h"
#include "Engine/Texture.h"
#include "Styling/SlateStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Framework/Docking/TabManager.h"
#include "IPConfig.h"
#include "SoundConverterLogic.h"


void SMainMenuWidget::Construct(const FArguments& InArgs)
{

	FSlateBrush WhiteImageBrush;
	WhiteImageBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.3f); // White color with opacity of 0.3

	dropVideoText = SNew(STextBlock)
		.Text(FText::FromString("Drop item here!"))
		.ColorAndOpacity(FLinearColor::White);

	title = SNew(STextBlock)
			.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf") , 15))
			.Text(FText::FromString("AI Motion Search"));
	
	IPConfig::ImagePath = "/Script/Engine.Texture2D'/Game/Sungjun/mixamo.mixamo'";
	USoundConverterLogic* ImageLibrary = NewObject<USoundConverterLogic>();
	//ImageLibrary->SearchImageFromUE("/Script/Engine.Texture2D'/Game/Sungjun/mixamo.mixamo'");

	const FSlateBrush* MyBrush = &( ImageLibrary->MySlateBrush );
	contentImage = SNew(SImage)
		.Image(MyBrush);
	/*TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("TEST_PostProductionStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("TEST_PostProduction")->GetBaseDir() / TEXT("Resources"));
	Style->GetBrush("TEST_PostProduction.PluginAction");


	Style->Set("TEST_PostProduction.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon")));*/

	ChildSlot
	[
		/*SNew(SDropTarget)
			.OnAllowDrop_Raw(this, &SMainMenuWidget::OnAllowDrop)
			.OnDropped(this, &SMainMenuWidget::OnDropVideo)
			[
				SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("NoBorder"))
					.Padding(4.0f)
.ColorAndOpacity(FColor::Blue)
.DesiredSizeScale(FVector2D(0.5f,0.5f))
					[
						SNew(STextBlock)
							.Text(FText::FromString("Drop items here!"))
							.ColorAndOpacity(FLinearColor::White)
					]*/


		//SNew(SBorder)
		//	.BorderImage(FCoreStyle::Get().GetBrush("NoBorder"))
		//	.ContentScale(FVector2D(1.0f, 0.7f))
		//	/*.OnMouseButtonDown_Lambda([]() {
		//		UE_LOG(LogTemp, Warning, TEXT("MouseButtonDown"));
		//		return FReply::Handled();
		//	})
		//	.OnMouseButtonUp_Lambda([]() {
		//		UE_LOG(LogTemp, Warning, TEXT("MouseButtonUp"));
		//		return FReply::Handled();
		//	})*/
		//	.Padding(0,15)
		//	[
		SNew(SHorizontalBox)
		
		+SHorizontalBox::Slot()
		.Padding(40)
		[
			contentImage.ToSharedRef()
		]

		+SHorizontalBox::Slot()
		[
				SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.VAlign(EVerticalAlignment::VAlign_Center)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.Padding(0 ,30,0,0)
					[
						title.ToSharedRef()
					]

					+ SVerticalBox::Slot()
.Padding(30)
					[
						SNew(SBorder)
							.BorderBackgroundColor(FLinearColor::Gray)
							.Padding(30)
.VAlign(VAlign_Center)
.HAlign(HAlign_Center)
.DesiredSizeScale(FVector2D(0.6, 0.6))
						[
							SNew(SDropTarget)
								.OnAllowDrop_Raw(this , &SMainMenuWidget::OnAllowDrop)
								.OnDropped(this , &SMainMenuWidget::OnDropVideo)
								[
									SNew(SBox)
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										[
											dropVideoText.ToSharedRef()
										]
								]
						]
					]

					+ SVerticalBox::Slot()
					.Padding(20 , 10)
					.AutoHeight()
					.HAlign(HAlign_Right)
					[
						SNew(SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Right)
							.OnClicked(this , &SMainMenuWidget::OnVideoUploadFileClicked)
							[
								SNew(STextBlock)
									.Text(FText::FromString("video Upload"))
							]

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

		dropText->SetText(FText::FromString("Click *Upload File* Button To Get Animation"));

		SetAssetPath(*FileOperation->GetFiles().begin());
		
		return FReply::Handled();
	}

	return FReply::Unhandled();

}

FReply SMainMenuWidget::OnDropImage(const FGeometry& MyGeometry , const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FDragDropOperation> DragDropOperation = DragDropEvent.GetOperation();

	if ( DragDropOperation.IsValid() && DragDropOperation->IsOfType<FAssetDragDropOp>() )
	{
		UE_LOG(LogTemp , Warning , TEXT("Can't Drop UASSET"));
	}
	else if ( DragDropOperation.IsValid() && DragDropOperation->IsOfType<FExternalDragOperation>() )
	{
		TSharedPtr<FExternalDragOperation> FileOperation = StaticCastSharedPtr<FExternalDragOperation>(DragDropOperation);

		UE_LOG(LogTemp , Warning , TEXT("Dropped file path num: %s") , **FileOperation->GetFiles().begin());

		dropText->SetText(FText::FromString("Click *Upload File* Button To Get 3D Object"));

		SetAssetPath(*FileOperation->GetFiles().begin());

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SMainMenuWidget::OnUploadFileClicked()
{
	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FString base64Info = FileUpload->UploadFile(AssetPath);

	FString description = "cream_cake";

	dropText->SetText(FText::FromString("Downloading starts! It may take a few minutes"));

	// Request whether Or not Motion Matching is finished
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("imageEncodingString", *base64Info);
	RequestObj->SetStringField("description", *description);

	// 3d Object Test
	FString RequestBody;
	FString URL = IPConfig::StaticVariable + "/image/upload";
	
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	UE_LOG(LogTemp, Warning, TEXT("Token : %s"), *IPConfig::Token);
	FString BearerToken = "Bearer " + IPConfig::Token;

	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//Request->SetHeader(TEXT("Authorization"), BearerToken);
	Request->SetContentAsString(RequestBody);
	//Request->OnProcessRequestComplete().BindRaw(this, &SMainMenuWidget::OnGetMMDone);
	Request->ProcessRequest();

	return FReply::Handled();
}

FReply SMainMenuWidget::OnVideoUploadFileClicked()
{
	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FString base64Info = FileUpload->UploadFile(AssetPath);

	dropVideoText->SetText(FText::FromString("Downloading starts! It may take a few minutes"));

	// Request whether Or not Motion Matching is finished
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("fileName" , *base64Info);

	FString RequestBody;
	FString URL = IPConfig::StaticVariable + "/view/video";


	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj , Writer);

	UE_LOG(LogTemp , Warning , TEXT("OnVideoUploadFileClicked Token : %s") , *IPConfig::Token);
	FString BearerToken = "Bearer " + IPConfig::Token;

	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	//Request->SetHeader(TEXT("Authorization"), BearerToken);
	Request->SetContentAsString(RequestBody);
	Request->OnProcessRequestComplete().BindRaw(this, &SMainMenuWidget::OnGetMMDone);
	Request->ProcessRequest();

	return FReply::Handled();
}


void SMainMenuWidget::OnGetMMDone(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		// MotionMenu Tab에 이벤트 전달, 웹 페이지의 visibility를 true로 변경해준다.
		//if(OnGetResponse.IsBound()) OnGetResponse.Execute();
		UE_LOG(LogTemp, Warning, TEXT("Successfully Get Response : %d"), Response->GetResponseCode());

		//dropText->SetText(FText::FromString("Downloading Completed, Drop item here again!"));


		  // 다른 방식
		TSharedPtr<SDockTab> motionTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("Motion Tab"));
		if(!motionTab.IsValid()) {
			FGlobalTabmanager::Get()->TryInvokeTab(FName("Motion Tab"));
			FGlobalTabmanager::Get()->SetActiveTab(motionTab);
		}
		else
		{
			// 만약 이미 tab이 활성화 되어있다면 reload를 한다. 
			//motionTab->RequestCloseTab();
			FGlobalTabmanager::Get()->TryInvokeTab(FName("Motion Tab"));
			/*TSharedPtr<SDockTab> reMotionTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("Motion Tab"));*/
			//FGlobalTabmanager::Get()->SetActiveTab(motionTab);
			UE_LOG(LogTemp, Warning, TEXT("Reload"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Get Response From Motion Matching Downloading Process"));
	}
}
