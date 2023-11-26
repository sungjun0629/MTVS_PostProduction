// Fill out your copyright notice in the Description page of Project Settings.


#include "SImageConverter.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "FileToBase64Uploader_Plugin.h"
#include "HttpModule.h"
#include "IPConfig.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "EditorWidgets/Public/SDropTarget.h"
#include "FileToStorageDownloader_Plugin.h"
#include "JsonParseLibrary_Plugin.h"

void SImageConverter::Construct(const FArguments& InArgs)
{
	FSlateBrush WhiteImageBrush;
	WhiteImageBrush.TintColor = FLinearColor(1.0f , 1.0f , 1.0f , 0.3f); // White color with opacity of 0.3


	dropText = SNew(STextBlock)
		.Text(FText::FromString("Drop image here!"))
		.ColorAndOpacity(FLinearColor::White);

	title = SNew(STextBlock)
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf") , 15))
		.Text(FText::FromString("3D Generator"));

	IPConfig::ImagePath = "/Script/Engine.Texture2D'/Game/Sungjun/mask.mask'";
	USoundConverterLogic* ImageLibrary = NewObject<USoundConverterLogic>();
	//ImageLibrary->SearchImageFromUE("/Script/Engine.Texture2D'/Game/Sungjun/mixamo.mixamo'");

	const FSlateBrush* MyBrush = &( ImageLibrary->MySlateBrush );


	ChildSlot
		[

			SNew(SHorizontalBox)

			+SHorizontalBox::Slot()
			[
				SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.VAlign(EVerticalAlignment::VAlign_Center)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.Padding(0 , 30 , 0 , 0)
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
							.DesiredSizeScale(FVector2D(0.6 , 0.6))
							[
								SNew(SDropTarget)
									.OnAllowDrop_Raw(this , &SImageConverter::OnAllowDrop)
									.OnDropped(this , &SImageConverter::OnDropImage)
									[
										SNew(SBox)
											.HAlign(HAlign_Center)
											.VAlign(VAlign_Center)
											[
												dropText.ToSharedRef()
											]
									]
							]
					]


					+ SVerticalBox::Slot()
					.Padding(0 , 10 , 20 , 0)
					.AutoHeight()
					[
						SNew(SHorizontalBox)

							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Right)
							[
								SNew(STextBlock)
									.Text(FText::FromString("Description : "))
							]

							+ SHorizontalBox::Slot()
							[
								SNew(SEditableText)
									.Text(FText::FromString("Input"))
									.OnTextCommitted_Lambda([ = ] (const FText& InText , ETextCommit::Type InCommitType) {
									description = InText.ToString();
										})
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
							.OnClicked(this , &SImageConverter::OnVideoUploadFileClicked)
							[
								SNew(STextBlock)
									.Text(FText::FromString("image Convert"))
							]

					]
			]



			+ SHorizontalBox::Slot()
			.Padding(40)
			[
				SNew(SImage)
					.Image(MyBrush)
			]
		];
}

bool SImageConverter::OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation)
{
	return true;
}

FReply SImageConverter::OnDropImage(const FGeometry& MyGeometry , const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FDragDropOperation> DragDropOperation = DragDropEvent.GetOperation();

	if ( DragDropOperation.IsValid() && DragDropOperation->IsOfType<FExternalDragOperation>() )
	{
		TSharedPtr<FExternalDragOperation> FileOperation = StaticCastSharedPtr<FExternalDragOperation>(DragDropOperation);

		UE_LOG(LogTemp , Warning , TEXT("Dropped file path num: %s") , **FileOperation->GetFiles().begin());

		dropText->SetText(FText::FromString("Click *Upload File* Button To Get 3D Object"));

		SetAssetPath(*FileOperation->GetFiles().begin());

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SImageConverter::OnVideoUploadFileClicked()
{
	if (GetAssetPath().IsEmpty() ) return FReply::Unhandled();

	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FString base64Info = FileUpload->UploadFile(AssetPath);

	// Request whether Or not Motion Matching is finished
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("imageEncodingString" , *base64Info);
	RequestObj->SetStringField("description" , *description);

	FString RequestBody;
	FString URL = IPConfig::StaticVariable + "/image/upload";

	dropText->SetText(FText::FromString("Downloading starts! It may take a few minutes"));

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj , Writer);

	FString BearerToken = "Bearer " + IPConfig::Token;

	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	//Request->SetHeader(TEXT("Authorization"), BearerToken);
	Request->SetContentAsString(RequestBody);
	Request->OnProcessRequestComplete().BindRaw(this, &SImageConverter::OnGet3DImage);
	Request->ProcessRequest();

	return FReply::Handled();
}

void SImageConverter::OnGet3DImage(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		UFileToStorageDownloader_Plugin* StorageDownload;
		UJsonParseLibrary_Plugin* jsonParser = NewObject<UJsonParseLibrary_Plugin>();
		/*const FDateTime Now = FDateTime::Now();
		const FString DateTimeString = Now.ToString(TEXT("%Y%m%d%H%M%S")); const FDateTime Now = FDateTime::Now();
		const FString DateTimeString = Now.ToString(TEXT("%Y%m%d%H%M%S"));*/
		const FString res	= Response->GetContentAsString();

		TArray<FString> parsedData = jsonParser->JsonParse3DImage(res);

		FString SavePath = "D:\\DownTest\\";
		//SavePath.Append(DateTimeString);
		SavePath.Append(description);
		TArray<FString> extensions;

		extensions.Add("_albedo.png");
		extensions.Add("_meshMat.mtl");
		extensions.Add("_meshObj.obj");
	

		for ( int32 i = 0 ; i < 3; i++ )
		{
			FString url = parsedData[i];
			FString storagePath = SavePath + extensions[i];

			StorageDownload->DownloadFileToStorage(url , storagePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);
		}
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("Failed to Get 3D Asset"));
	}
}
