// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundConverterLogic.h"
#include "FileToBase64Uploader_Plugin.h"
#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "IPConfig.h"
#include "JsonParseLibrary_Plugin.h"
#include "FileToStorageDownloader_Plugin.h"


USoundConverterLogic::USoundConverterLogic()
{

}

USoundConverterLogic::~USoundConverterLogic()
{

}

void USoundConverterLogic::ConvertedSoundDownload(FString loadedAsset)
{
    UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
    FString base64Info = FileUpload->UploadFile(loadedAsset);

    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

    FString ModelName = "dahyun";
    RequestObj->SetStringField("fileName" , *base64Info);
    RequestObj->SetStringField("modelName" , *ModelName);

    FString RequestBody;
    FString URL = IPConfig::StaticVariable + "/view/voice";


    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestObj , Writer);

    UE_LOG(LogTemp , Warning , TEXT("Token : %s") , *IPConfig::Token);
    FString BearerToken = "Bearer " + IPConfig::Token;

    Request->SetURL(URL);
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
    Request->SetHeader(TEXT("Authorization") , BearerToken);
    Request->SetContentAsString(RequestBody);
    Request->OnProcessRequestComplete().BindUObject(this , &USoundConverterLogic::OnDownloadConvertedVoice);
    Request->ProcessRequest();

}

void USoundConverterLogic::OnDownloadConvertedVoice(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully)
{
    if ( bConnectedSuccessfully )
    {
        UJsonParseLibrary_Plugin* jsonParser = NewObject<UJsonParseLibrary_Plugin>();
        FString res = Response->GetContentAsString();
        FString parsedData = jsonParser->JsonParse(res);


        UFileToStorageDownloader_Plugin* StorageDownload;
        FString url = parsedData;
        FString SavePath = "D:\\DownTest\\";

        StorageDownload->DownloadFileToStorage(url , SavePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);
    }
}
