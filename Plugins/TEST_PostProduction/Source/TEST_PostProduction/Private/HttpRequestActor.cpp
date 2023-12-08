// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestActor.h"
#include "HttpModule.h"
//#include "HttpGameModeBase.h"
//#include "JsonParseLibrary.h"
#include "ImageUtils.h"
#include "JsonParseLibrary_Plugin.h"
#include "Dom/JsonObject.h"
#include "FileToBase64Uploader_Plugin.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "IPConfig.h"
#include "Containers/Array.h"
#include "Dom/JsonValue.h"
#include "ProjectTable.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "SoundConverterLogic.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/Base64.h"
#include "FileToStorageDownloader_Plugin.h"
#include "Misc/FileHelper.h"
#include "ImportExportDataTable.h"
#include "EUW_ScriptSystem.h"


UHttpRequestActor::UHttpRequestActor()
{
	OnFileToStorageDownloadCompleteDelegate.BindDynamic(this , &UHttpRequestActor::SuccessCSVDownload);
}

UHttpRequestActor::~UHttpRequestActor()
{
	OnFileToStorageDownloadCompleteDelegate.Unbind();
}

void UHttpRequestActor::PostProjectRequest(const FString ProjectName , const FString scriptPath, const FString ProjectDes , TArray<FWorkerInfo> StaffInfo , const FString ImagePath)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	const FString URL = IPConfig::StaticVariable + "/group/project/upload";

	// For Make imageBase64
	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FString ImageBase64 = FileUpload->UploadFile(ImagePath);
	FString CSVBase64 = FileUpload->UploadFile(scriptPath);
	/*FString CSVBase64;
	TArray<uint8> FileData;

	if ( !FFileHelper::LoadFileToArray(FileData , *scriptPath) )
	{
		UE_LOG(LogTemp , Error , TEXT("Failed to load file: %s") , *scriptPath);
	}
	CSVBase64 = FBase64::Encode(FileData);*/

	// Crate a JSON object
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for ( int32 i = 0; i < StaffInfo.Num(); i++ )
	{
		TSharedPtr<FJsonObject> StructObject = MakeShared<FJsonObject>();
		StructObject->SetStringField(TEXT("email") , *StaffInfo[i].email);
		StructObject->SetStringField(TEXT("staffRole") , "director");
		TSharedPtr<FJsonValueObject> JsonValue = MakeShared<FJsonValueObject>(StructObject);
		JsonArray.Add(JsonValue);
	}

	// Create an array for avatarName
	TArray<TSharedPtr<FJsonValue>> AvatarNamesArray;

	// Add individual strings to the array
	AvatarNamesArray.Add(MakeShareable(new FJsonValueString(TEXT("for"))));
	AvatarNamesArray.Add(MakeShareable(new FJsonValueString(TEXT("stream"))));


	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("projectName" , *ProjectName);
	RequestObj->SetStringField("description" , *ProjectDes);
	RequestObj->SetArrayField("staffs", JsonArray);
	RequestObj->SetArrayField("avatarName", AvatarNamesArray);
	RequestObj->SetStringField("script", *CSVBase64);
	RequestObj->SetStringField("poster" , *ImageBase64);


	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj , Writer);

	Request->SetURL(URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	Request->SetContentAsString(RequestBody);
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnPostProjectInfo);
	Request->ProcessRequest();
}

void UHttpRequestActor::PostSceneCard(int32 projectID , const FString story , FString levelLocation , FString imagePath)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	const FString URL = IPConfig::StaticVariable + "/group/scene/upload";

	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FString ImageBase64 = FileUpload->UploadFile(imagePath);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetNumberField("scriptId" , projectID);
	RequestObj->SetStringField("story" , *story);
	RequestObj->SetStringField("levelPosition" , *levelLocation);
	RequestObj->SetStringField("thumbNail" , *ImageBase64);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj , Writer);

	Request->SetURL(URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	Request->SetContentAsString(RequestBody);
	//Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnPostProjectInfo);
	Request->ProcessRequest();
}

void UHttpRequestActor::GetAllProject()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	const FString URL = IPConfig::StaticVariable + "/group/project/all";

	// GET처리 
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnReciveAllProject);
	Request->ProcessRequest();
}


void UHttpRequestActor::GetScriptCSV(int32 projectId)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	FString URL = IPConfig::StaticVariable + "/group/scene/";
	URL.Append(FString::Printf(TEXT("%d") , projectId));


	// GET처리 
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnGetScriptCSVDownload);
	Request->ProcessRequest();
}

void UHttpRequestActor::GetParticularProject(int32 number)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	FString URL = IPConfig::StaticVariable + "/group/project/";
	// 프로젝트의 고유 번호를 통해 접근한다. 
	URL.Append(FString::Printf(TEXT("%d") , number));

	// GET처리 
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnReciveParticularProject);
	Request->ProcessRequest();
}


void UHttpRequestActor::GetParticularSceneCard(int32 projectID , int32 sceneNo)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	FString URL = IPConfig::StaticVariable + "/group/scene/";
	// 프로젝트와 씬카드의 고유 번호를 통해 접근한다. 
	URL.Append(FString::Printf(TEXT("%d/%d") , projectID, sceneNo));

	// GET처리 
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnGetParticularSceneCard);
	Request->ProcessRequest();
}

FSlateBrush UHttpRequestActor::GetImageTexture(int32 projectID)
{
	/*FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(url);
	Request->SetHeader(TEXT("Content-Type") , TEXT("image/jpg"));
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnGetImageTexture);
	Request->ProcessRequest();*/

	FString DataTablePath = "/Script/Engine.DataTable'/Game/Sungjun/ProjectInfo.ProjectInfo'";
	UDataTable* LoadedDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass() , nullptr , *DataTablePath));
	FSlateBrush MySlateBrush;

	TableRows.Empty();
	LoadedDataTable->GetAllRows<FProjectTable>("random" , TableRows);

	for ( FProjectTable* TableRow : TableRows )
	{
		if ( TableRow->projectID == projectID )
		{
			/*ConstructorHelpers::FObjectFinder<UTexture2D> imageFinder(*(TableRow->imagePath));
			if ( imageFinder.Succeeded() )
			{
				MySlateBrush.SetResourceObject(imageFinder.Object);
			}*/

			// 테이블을 통한 이미지 관리
			IPConfig::ImagePath = TableRow->imagePath;

			USoundConverterLogic* ImageLibrary = NewObject<USoundConverterLogic>();
			MySlateBrush = (ImageLibrary->MySlateBrush );


			return MySlateBrush;
		}
	}

	return MySlateBrush;
}

void UHttpRequestActor::GetCSVDownload()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	FString URL = IPConfig::StaticVariable + "/group/memo";

	// GET처리 
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnGetCSVDownloadURL);
	Request->ProcessRequest();
}

void UHttpRequestActor::SuccessCSVDownload(EDownloadToStorageResult_Plugin Result)
{
	if ( Result == EDownloadToStorageResult_Plugin::Success )
	{
		UE_LOG(LogTemp , Warning , TEXT("Success"));
	}
	else if ( Result == EDownloadToStorageResult_Plugin::SucceededByPayload )
	{
		UE_LOG(LogTemp , Warning , TEXT("SucceededByPayload"));

	}
	else if ( Result == EDownloadToStorageResult_Plugin::Cancelled )
	{
		UE_LOG(LogTemp , Warning , TEXT("Cancelled"));

	}
	else if ( Result == EDownloadToStorageResult_Plugin::DownloadFailed )
	{
		UE_LOG(LogTemp , Warning , TEXT("DownloadFailed"));

	}
	else if ( Result == EDownloadToStorageResult_Plugin::SaveFailed )
	{
		UE_LOG(LogTemp , Warning , TEXT("SaveFailed"));

	}
	else if ( Result == EDownloadToStorageResult_Plugin::DirectoryCreationFailed )
	{
		UE_LOG(LogTemp , Warning , TEXT("DirectoryCreationFailed"));

	}
	else if ( Result == EDownloadToStorageResult_Plugin::InvalidURL )
	{
		UE_LOG(LogTemp , Warning , TEXT("InvalidURL"));

	}
	else if ( Result == EDownloadToStorageResult_Plugin::InvalidSavePath )
	{
		UE_LOG(LogTemp , Warning , TEXT("InvalidSavePath"));

	}


	// 다운로드 받은 엑셀과 현재의 엑셀을 비교하고 합치는 작업을 진행한다.
	// 결과물은 Test3.csv로 도출된다.
	if(!isScript) IPConfig::MemoTableEditor->CompareCSVFiles("D:\\DownTest\\Test.csv" , "D:\\DownTest\\Test2.csv");
	else {
		isScript = false;

		FString OutputPath = "D:\\DownTest\\TempScript.csv";
		bool isSuccess = false;
		FString OutInfoMessage;

		// 언리얼내로 해당 CSV 파일을 Import를 실시한다.
		UExcelImportExportDataTable::ImportDataTableFromCSV(OutputPath , "/Game/Shoplifters_ScriptSheet" , FScriptDT::StaticStruct() , isSuccess , OutInfoMessage);
		
		// Import가 제대로 수행되었다면.. 
		if ( isSuccess )
		{
			UE_LOG(LogTemp,Warning,TEXT("Script CSV Import Success... Let's Parsing"));
			OnScriptSuccessDelegate.Broadcast();
		}
		
	}
}


void UHttpRequestActor::PostCSVToStorage(FString savePath)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	const FString URL = IPConfig::StaticVariable + "/group/memo/upload";

	UE_LOG(LogTemp,Warning,TEXT("savePath : %s"), *savePath);
	// For Make imageBase64
	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FString CSVBase64 = FileUpload->UploadFile(savePath);


	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("csvFile" , *CSVBase64);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj , Writer);

	Request->SetURL(URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	Request->SetContentAsString(RequestBody);
	Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnPostCSVFile);
	Request->ProcessRequest();
}

void UHttpRequestActor::OnReciveAllProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 파싱을 한다. 
		FString response = Response->GetContentAsString();
		TArray<FProjectUnit> allProjectParsedData = UJsonParseLibrary_Plugin::JsonProjectParse(response);

		// 프로젝트은 배열 형태로 Object를 갖는다.
		// 리턴을 해서 블루프린트에서 처리를 해준다. 
		projectArray = allProjectParsedData;
		OnreciveAllProjectDeletage.Broadcast();
	}
	else
	{
	}
}

void UHttpRequestActor::OnReciveParticularProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 파싱을 한다. 
		FString response = Response->GetContentAsString();
		FProjectInfo ParticularProjectParsedData = UJsonParseLibrary_Plugin::JsonPraticularProjectParse(response);
		// 각각의 프로젝트은 Object 형식으로 프로젝트의 정보를 갖고 있다.  
		projectInfoArray = ParticularProjectParsedData;

		OnGetParticularProjectDelegate.Broadcast(projectInfoArray);
	}
	else
	{
	}
}

void UHttpRequestActor::OnGetImageTexture(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		TArray<uint8> texBites = Response->GetContent();

		realTex = FImageUtils::ImportBufferAsTexture2D(texBites);
		
		OnGetImageTextureDelegate.Broadcast();
	}
	else
	{

	}
}




void UHttpRequestActor::OnPostProjectInfo(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		OnSuccessPostDelegate.Broadcast();
	}
	else 
	{

	}
}

void UHttpRequestActor::OnPostCSVFile(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		UE_LOG(LogTemp,Warning,TEXT("Success to Post CSV File"));

		

	}
	else
	{

	}
}

void UHttpRequestActor::OnGetCSVDownloadURL(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		
		UFileToStorageDownloader_Plugin* StorageDownload;
		FString SavePath = "D:\\DownTest\\Test2.csv";
		FString response = Response->GetContentAsString();
		FString URL = UJsonParseLibrary_Plugin::JsonParseToGetURL(response);
		if(URL!="")
		{
			StorageDownload->DownloadFileToStorage(URL , SavePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);
			UE_LOG(LogTemp,Warning,TEXT("Success to Get csv File"));
		}
		else
		{// 만약 다운로드 할 url이 없다면! 즉, 스토리지에 아무것도 없다면 임의로 다운로드 파일을 만들어준다.
			FString OutputContent;
			FString OutputPath = "D:\\DownTest\\Test2.csv";

			FFileHelper::SaveStringToFile(OutputContent , *OutputPath);

		 // 그리고 빈 값의 csv 파일과 비교를 실시한다.
			IPConfig::MemoTableEditor->CompareCSVFiles("D:\\DownTest\\Test.csv" , "D:\\DownTest\\Test2.csv");
		}
	}
	else
	{

	}
}

void UHttpRequestActor::OnGetScriptCSVDownload(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		
		UFileToStorageDownloader_Plugin* StorageDownload;
		FString response = Response->GetContentAsString();
		FString SavePath = "D:\\DownTest\\TempScript.csv";
		FString URL = UJsonParseLibrary_OnScriptSuccessDelegatePlugin::JsonParseToGetURL(response);
		
		if ( !URL.IsEmpty() )
		{
			isScript = true;
			StorageDownload->DownloadFileToStorage(URL , SavePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);
			UE_LOG(LogTemp , Warning , TEXT("Success to Get Script csv File"));
			// 다운로드가 완료된다면 언리얼에 Import를 해주어야한다. 
		}

	}
	else
	{

	}
}

void UHttpRequestActor::OnGetParticularSceneCard(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		FString response = Response->GetContentAsString();
		// Response값을 파싱해준다. 
		FSceneCardInfo parsedInfo = UJsonParseLibrary_Plugin::JsonParseToGetSceneInfo(response);
		// 파싱해준값을 전역 변수에 저장한다. 
		sceneInfo = parsedInfo;

		OnGetParticularSceneCardDelegate.Broadcast();
	}
	else
	{

	}
}

FString UHttpRequestActor::OnImportButtonClicked()
{
	FString path = "";

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if ( DesktopPlatform )
	{
		// Set the default path to the project directory
		FString DefaultPath = FPaths::ProjectDir();

		// Open the folder picker dialog
		const void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();
		TArray<FString> OutFolderNames;

		bool bOpened = DesktopPlatform->OpenFileDialog(
			ParentWindowPtr ,
			TEXT("Select a folder") ,
			DefaultPath ,
			TEXT("") ,
			TEXT("All Files (*.*)|*.*") ,
			EFileDialogFlags::None ,
			OutFolderNames
		);

		// Process the selected folder
		if ( bOpened && OutFolderNames.Num() > 0 )
		{
			FString SelectedDirectory = OutFolderNames[ 0 ];
			path = SelectedDirectory;
			// You can perform further actions with the selected directory here.
		}
	}

	return path;
}

UTexture2D* UHttpRequestActor::Base64ToImage(FString Base64String)
{
	TArray<uint8> ByteArray;

	// Convert FString to UTF-8 encoded string
	FString UTF8String = Base64String; // Make a copy to ensure the original string isn't modified
	FTCHARToUTF8 UTF8Converter(*UTF8String); // Use FTCHARToUTF8 to convert FString to UTF-8

	// Check if the conversion succeeded
	if ( UTF8Converter.Length() > 0 )
	{
		const uint8* Data = reinterpret_cast< const uint8* >( UTF8Converter.Get() ); // Get the UTF-8 data
		ByteArray.Append(Data , UTF8Converter.Length()); // Append the data to the byte array
	}


	UTexture2D* Texture = nullptr;
	FString Base64DecodedString;
	bool isDecode = FBase64::Decode(*Base64String , ByteArray , EBase64Mode::Standard);
	if(isDecode)
	{
		UE_LOG(LogTemp,Warning,TEXT("Decoding"))
		Texture = FImageUtils::ImportBufferAsTexture2D(ByteArray);
		if(Texture!=nullptr)
		{
			Texture->MipGenSettings = TMGS_NoMipmaps;
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;
			Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
			Texture->SRGB = false;
			Texture->Filter = TextureFilter::TF_Nearest;
			Texture->UpdateResource();
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Decoding fail"))
	}

	return Texture;
}

#pragma region other

//// Json 파일 저장하기
//void AHttpRequestActor::SaveJson(const FString jsonData)
//{
//	// 만일, 지정된 이름의 폴더가 없으면 해당 폴더를 만든다.
//	FPlatformFileManager& fileManager = FPlatformFileManager::Get();
//	IPlatformFile& platformFile = fileManager.GetPlatformFile();
//
//	FString dirPath = FPaths::ProjectContentDir() + "/JsonData";
//	if ( !platformFile.DirectoryExists(*dirPath) )
//	{
//		platformFile.CreateDirectory(*dirPath);
//	}
//
//	// json 데이터를 파일로 저장한다.
//	FString fullPath = dirPath + "/MyJson.json";
//	UE_LOG(LogTemp , Warning , TEXT("save Path: %s") , *fullPath);
//	bool bIsSaved = FFileHelper::SaveStringToFile(jsonData , *fullPath);
//	gm->SetLogText(FString::Printf(TEXT("%s") , bIsSaved ? *FString("Json Saved Successfully!") : *FString("Failed saving file...")));
//}
//
//// 이미지 요청 함수
//void AHttpRequestActor::GetImage(const FString url)
//{
//	TSharedRef<IHttpRequest> req = FHttpModule::Get().CreateRequest();
//	req->SetURL(url);
//	req->SetVerb("GET");
//	req->SetHeader(TEXT("Content-Type") , TEXT("image/jpeg"));
//	req->OnProcessRequestComplete().BindUObject(this , &AHttpRequestActor::OnGetImageData);
//	req->ProcessRequest();
//}
//
//void AHttpRequestActor::OnGetImageData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully)
//{
//	if ( bConnectedSuccessfully )
//	{
//		TArray<uint8> texBites = Response->GetContent();
//		FString imagePath = FPaths::ProjectPersistentDownloadDir() + "/MyGetImage.jpg";
//		FFileHelper::SaveArrayToFile(texBites , *imagePath);
//		UTexture2D* realTex = FImageUtils::ImportBufferAsTexture2D(texBites);
//		gm->SetImageTexture(realTex);
//		gm->SetLogText("Successfully!");
//	}
//	else
//	{
//		gm->SetLogText(FString::Printf(TEXT("Failed...\nResponse Code: %d") , Response->GetResponseCode()));
//	}
//}
//
//void AHttpRequestActor::SaveImage(const UTexture2D* tex)
//{
//
//}
//
//// 텍스쳐 Post 함수
//void AHttpRequestActor::PostImage(const FString url , const UTexture2D* tex)
//{
//	TArray<uint8> convertedImage;
//
//	// 텍스쳐의 각 픽셀 컬러 정보를 배열에 담는다.
//	FTexture2DMipMap mipData = tex->GetPlatformData()->Mips[ 0 ];
//
//	TArray<FColor> imgArr;
//	int32 width = mipData.SizeX;
//	int32 height = mipData.SizeY;
//	imgArr.AddUninitialized(width * height * sizeof(FColor));
//
//	void* pixelData = mipData.BulkData.Lock(LOCK_READ_ONLY);
//
//	if ( pixelData != nullptr )
//	{
//		FMemory::Memcpy(imgArr.GetData() , pixelData , imgArr.Num());
//	}
//	mipData.BulkData.Unlock();
//
//	// 텍스쳐의 픽셀 컬러 배열을 이미지 포맷으로 압축한다.
//	FImageUtils::ThumbnailCompressImageArray(width , height , imgArr , convertedImage);
//
//	// 이미지 바이트 배열을 포스트 한다.
//	TSharedRef<IHttpRequest> req = FHttpModule::Get().CreateRequest();
//	req->SetURL(url);
//	req->SetVerb("POST");
//	req->SetHeader(TEXT("Content-Type") , TEXT("image/jpeg"));
//	req->SetContent(convertedImage);
//	req->OnProcessRequestComplete().BindUObject(this , &AHttpRequestActor::OnPostTextureData);
//	req->ProcessRequest();
//}
//
//void AHttpRequestActor::OnPostTextureData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully)
//{
//	gm->SetLogText(FString::Printf(TEXT("%s") , bConnectedSuccessfully ? *Response->GetContentAsString() : *FString(TEXT("Reponse code: %d") , Response->GetResponseCode())));
//}

#pragma endregion
