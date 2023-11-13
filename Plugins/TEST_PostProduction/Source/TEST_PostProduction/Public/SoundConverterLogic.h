// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
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
	void ConvertedSoundDownload(FString loadedAsset);

	void OnDownloadConvertedVoice(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);


	FSlateBrush MySlateBrush;
	// 이미지 테스트용
	const FSlateBrush* SearchImageFromUE(FString imagePath);
};
