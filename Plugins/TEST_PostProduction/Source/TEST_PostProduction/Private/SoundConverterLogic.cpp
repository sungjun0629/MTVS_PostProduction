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
#include "UObject/ConstructorHelpers.h"


USoundConverterLogic::USoundConverterLogic()
{
    ConstructorHelpers::FObjectFinder<UTexture2D> imageFinder(TEXT("/Game/Sungjun/Images/storyBoard1"));

    if ( imageFinder.Succeeded() )
    {
        MySlateBrush.SetResourceObject(imageFinder.Object);
    }
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
    UE_LOG(LogTemp , Warning , TEXT("Base64 : %s") , *base64Info);
    FString BearerToken = "Bearer " + IPConfig::Token;

    Request->SetURL(URL);
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type") , TEXT("application/json"));
    //Request->SetHeader(TEXT("Authorization") , BearerToken);
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

        const FDateTime Now = FDateTime::Now();
        const FString DateTimeString = Now.ToString(TEXT("%Y%m%d%H%M%S"));;

        UFileToStorageDownloader_Plugin* StorageDownload;
        FString url = parsedData;
        
        UE_LOG(LogTemp,Warning,TEXT("url : %s"),*url);
        FString SavePath = "D:\\DownTest\\";
        SavePath.Append(DateTimeString);
        SavePath.Append(TEXT("_ConvertedVoice.wav"));

        StorageDownload->DownloadFileToStorage(url , SavePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);
    }
}

const FSlateBrush* USoundConverterLogic::SearchImageFromUE(FString imagePath)
{
    //MySlateBrush.SetResourceObject(LoadObject<UObject>(nullptr , TEXT(" / Script / Engine.Texture2D'/Game/Sungjun/Images/storyBoard1.storyBoard1'")));

    ConstructorHelpers::FObjectFinder<UTexture2D> imageFinder(TEXT("/Game/Sungjun/Images/storyBoard1"));

    if ( imageFinder.Succeeded() )
    {
        MySlateBrush.SetResourceObject(imageFinder.Object);
    }

    /*UTexture2D* story = LoadObject<UTexture2D>(nullptr , TEXT("/Game/Sungjun/Images/storyBoard1"));

    if (story)
    {
        UE_LOG(LogTemp , Warning , TEXT("story is. not null"))
        MySlateBrush.SetResourceObject(story);
    }*/

    const FSlateBrush* MyBrush = &MySlateBrush;

    return MyBrush;
}
