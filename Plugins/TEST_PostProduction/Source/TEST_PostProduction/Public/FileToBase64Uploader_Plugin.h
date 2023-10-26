// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "FileToBase64Uploader_Plugin.generated.h"

/**
 * 
 */
UCLASS()
class UFileToBase64Uploader_Plugin : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FString UploadFile(FString FullFilePath);

	void OnPostData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void GetResult();


private:
	FString UploadFileName;
	FString FullFilePathString;

	void ProcessResponse(FString ResponseContent);

	FString ExtensionString(FString FilePathString);

	
};
