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


void UHttpRequestActor::SendRequest(const FString url)
{
	// 모듈을 생성하고 request 인스턴스를 생성한다.
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedPtr<IHttpRequest> req = httpModule.CreateRequest();

	// 요청하기 위한 정보를 설정한다.
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("Content-Type") , TEXT("application/json"));

	// 요청이 완료되었을 때 실행될 함수를 바인딩한다.
	//req->OnProcessRequestComplete().BindUFunction(this, FName("OnReceivedData"));
	req->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnReceivedData);

	// 서버에 요청을 보낸다.
	req->ProcessRequest();
}

void UHttpRequestActor::OnReceivedData(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp , Warning , TEXT("Received Data!"));

	if ( bConnectedSuccessfully )
	{
		FString res = Response->GetContentAsString();
		FString parsedData = UJsonParseLibrary_Plugin::JsonParse(res);
		//gm->SetLogText(parsedData);
		UE_LOG(LogTemp , Warning , TEXT("%s"), *parsedData);
	}
	else
	{
		//gm->SetLogText("Failed Response from server...");
		UE_LOG(LogTemp , Warning , TEXT("Failed Response from server..."));
	}
}

// POST 메소드 요청하기
void UHttpRequestActor::PostRequest(const FString url)
{
	//TMap<FString , FString> studentData;
	//studentData.Add("Name" , "AAA");
	//studentData.Add("Age" , "50");
	//studentData.Add("Height" , "200");

	//FString myJsonData = UJsonParseLibrary::MakeJson(studentData);
	////gm->SetLogText(myJsonData);

	//// 요청 설정
	//FHttpModule& httpModule = FHttpModule::Get();
	//TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	//req->SetURL(url);
	//req->SetVerb(TEXT("POST"));
	//req->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	//req->SetContentAsString(myJsonData);
	//req->OnProcessRequestComplete().BindUObject(this , &AHttpRequestActor::OnPostData);
	//req->ProcessRequest();
}

void UHttpRequestActor::PostProjectRequest(const FString ProjectName , const FString ProjectDes , TArray<FWorkerInfo> StaffInfo , const FString ImagePath)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	const FString URL = IPConfig::StaticVariable + "/group/project/upload";

	// For Make imageBase64
	UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
	FString ImageBase64 = FileUpload->UploadFile(ImagePath);

	// Crate a JSON object
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for ( int32 i = 0; i < StaffInfo.Num(); i++ )
	{
		TSharedPtr<FJsonObject> StructObject = MakeShared<FJsonObject>();
		StructObject->SetStringField(TEXT("email") , *StaffInfo[i].email);
		StructObject->SetStringField(TEXT("staffRole") , *StaffInfo[i].role);

		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(StructObject.ToSharedRef() , Writer);

		TSharedPtr<FJsonValueString> JsonValue = MakeShared<FJsonValueString>(JsonString);
		JsonArray.Add(JsonValue);
	}

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("projectName" , *ProjectName);
	RequestObj->SetStringField("description" , *ProjectDes);
	RequestObj->SetArrayField("staffs", JsonArray);
	RequestObj->SetStringField("poster" , *ImageBase64);


	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj , Writer);

	Request->SetURL("URL");
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
	Request->SetContentAsString(RequestBody);
	//Request->OnProcessRequestComplete().BindUObject(this , &UHttpRequestActor::OnPostData);
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


void UHttpRequestActor::OnReciveAllProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 파싱을 한다. 
		FString response = Response->GetContentAsString();
		TArray<FProjectUnit> allProjectParsedData = UJsonParseLibrary_Plugin::JsonProjectParse(response);

		UE_LOG(LogTemp,Warning,TEXT("%s"), *response);
		UE_LOG(LogTemp,Warning,TEXT("%d"), allProjectParsedData.Num());

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
		TArray<FProjectUnit> allProjectParsedData = UJsonParseLibrary_Plugin::JsonProjectParse(response);
		// 각각의 프로젝트은 Object 형식으로 프로젝트의 정보를 갖고 있다.  
	}
	else
	{

	}
}

// POST 요청 함수
void UHttpRequestActor::OnPostData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully)
{
	//if ( bConnectedSuccessfully )
	//{
	//	FString receivedData = Response->GetContentAsString();

	//	// 받은 데이터를 화면에 출력한다.
	//	gm->SetLogText(receivedData);

	//	// 받은 데이터를 파일로 저장한다.
	//	SaveJson(receivedData);
	//}
	//else
	//{
	//	// 요청 전송 상태 확인
	//	EHttpRequestStatus::Type status = Request->GetStatus();
	//	switch ( status )
	//	{
	//	case EHttpRequestStatus::NotStarted:
	//		break;
	//	case EHttpRequestStatus::Processing:
	//		break;
	//	case EHttpRequestStatus::Failed:
	//		break;
	//	case EHttpRequestStatus::Failed_ConnectionError:
	//		break;
	//	case EHttpRequestStatus::Succeeded:
	//		break;
	//	default:
	//		break;
	//	}

	//	// 응답 코드 확인
	//	int32 responseCode = Response->GetResponseCode();
	//	gm->SetLogText(FString::Printf(TEXT("Response Code: %d") , responseCode));

	//}
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
//		gm->SetLogText("Get Image Successfully!");
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
