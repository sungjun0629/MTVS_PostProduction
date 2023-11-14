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
#include "SGetWebAddress.h"
#include "AudioDevice.h"
#include "Engine/Engine.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "LevelSequence/Public/LevelSequence.h"


USoundConverterLogic::USoundConverterLogic()
{
    ConstructorHelpers::FObjectFinder<UTexture2D> imageFinder(*(IPConfig::ImagePath));

    if ( imageFinder.Succeeded() )
    {
        MySlateBrush.SetResourceObject(imageFinder.Object);
    }

    OnFileToStorageDownloadCompleteDelegate.BindDynamic(this , &USoundConverterLogic::SuccessDownload);
}

USoundConverterLogic::~USoundConverterLogic()
{
    OnFileToStorageDownloadCompleteDelegate.Unbind();
}

void USoundConverterLogic::ConvertedSoundDownload(FString loadedAsset, FString modelName)
{
    UFileToBase64Uploader_Plugin* FileUpload = NewObject<UFileToBase64Uploader_Plugin>();
    FString base64Info = FileUpload->UploadFile(loadedAsset);

    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

    FString ModelName = modelName;
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

        FString url = parsedData;

        getWebAddress = MakeShared<SGetWebAddress>();
        getWebAddress->ReloadAndGetURL(url);
        
		/*UE_LOG(LogTemp,Warning,TEXT("url : %s"),*url);

		FString SavePath = "D:\\DownTest\\";
		const FDateTime Now = FDateTime::Now();
		const FString DateTimeString = Now.ToString(TEXT("%Y%m%d%H%M%S"));;
		SavePath.Append(DateTimeString);
		SavePath.Append(TEXT("_ConvertedVoice.wav"));

		UFileToStorageDownloader_Plugin* StorageDownload;

		StorageDownload->DownloadFileToStorage(url , SavePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);

		OnFileToStorageDownloadCompleteDelegate.BindUFunction(this , "SuccessDownload");*/
    }
}

void USoundConverterLogic::DownloadVoice(FString url)
{
    UE_LOG(LogTemp , Warning , TEXT("url : %s") , *url);

    SavePath = "D:\\DownTest\\";
    const FDateTime Now = FDateTime::Now();
    const FString DateTimeString = Now.ToString(TEXT("%Y%m%d%H%M%S"));;
    SavePath.Append(DateTimeString);
    SavePath.Append(TEXT("_ConvertedVoice.wav"));

    UFileToStorageDownloader_Plugin* StorageDownload;

    StorageDownload->DownloadFileToStorage(url , SavePath , 15.f , "" , true , OnDownloadProgressDelegate , OnFileToStorageDownloadCompleteDelegate);

    FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice().GetAudioDevice();
    AudioDevice->Flush(nullptr);
}

void USoundConverterLogic::SuccessDownload(EDownloadToStorageResult_Plugin Result)
{
    if ( Result == EDownloadToStorageResult_Plugin::Success )
    {
		UE_LOG(LogTemp , Warning , TEXT("Success"));
        downloadedVoicePath = SavePath;
	}
    else
    {
		UE_LOG(LogTemp , Warning , TEXT("Fail"));
	}
}

const FSlateBrush* USoundConverterLogic::SearchImageFromUE(FString imagePath)
{
    //MySlateBrush.SetResourceObject(LoadObject<UObject>(nullptr , TEXT(" / Script / Engine.Texture2D'/Game/Sungjun/Images/storyBoard1.storyBoard1'")));

    ConstructorHelpers::FObjectFinder<UTexture2D> imageFinder(*imagePath);

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

void USoundConverterLogic::GetSequenceAsset()
{
    IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();


    TArray<FAssetData> AssetList;
    //AssetRegistry.GetAssetsByClass(ULevelSequence::StaticClass()->GetFName(), AssetList);
    AssetRegistry.GetAssetsByPath(FName("/Game/Sungjun/Sequence/") , AssetList);

    for ( const FAssetData& Asset : AssetList )
    {
        // Use the asset as needed
        ULevelSequence* LevelSequence = Cast<ULevelSequence>(Asset.GetAsset());
        if ( LevelSequence )
        {
            // Perform operations on the Level Sequence
            UE_LOG(LogTemp , Warning , TEXT("LevelSequence : %s") , *LevelSequence->GetName());
            Options.Add(MakeShareable(new FString(LevelSequence->GetName())));
        }
    }
}
