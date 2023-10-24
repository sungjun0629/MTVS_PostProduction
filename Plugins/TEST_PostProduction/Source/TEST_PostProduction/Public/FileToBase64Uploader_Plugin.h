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
	void UploadFile(FString FullFilePath);

	//UFUNCTION()
	//void OnPostData(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully);
	void OnPostData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

private:
	FString UploadFileName;

	//FString BoundaryLabel = FString();
	//FString BoundaryBegin = FString();
	//FString BoundaryEnd = FString();

	//FString AddData(FString Name, FString Value);

	void ProcessResponse(FString ResponseContent);
};
