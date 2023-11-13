// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FileToStorageDownloader_Plugin.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "SGetWebAddress.h"
#include "SoundConverterLogic.generated.h"


/**
 * 
 */
 UCLASS(BlueprintType, Category = "SoundConverter")
class USoundConverterLogic : public UObject
{
	GENERATED_BODY()
	
public:
	USoundConverterLogic();
	~USoundConverterLogic();
	
	FOnDownloadProgress OnDownloadProgressDelegate;

	FOnFileToStorageDownloadComplete OnFileToStorageDownloadCompleteDelegate;

	UFUNCTION(BlueprintCallable, Category = "SoundConverter")
	void ConvertedSoundDownload(FString loadedAsset, FString modelName);

	UFUNCTION(BlueprintCallable, Category = "SoundConverter")
	void SuccessDownload(EDownloadToStorageResult_Plugin Result);

	void OnDownloadConvertedVoice(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);

	void DownloadVoice(FString url);

	FSlateBrush MySlateBrush;
	// 이미지 테스트용
	const FSlateBrush* SearchImageFromUE(FString imagePath);

	void GetSequenceAsset();
	TArray<TSharedPtr<FString>> Options;

	TSharedPtr<SGetWebAddress> getWebAddress;
};
