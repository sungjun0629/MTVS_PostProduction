#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "FileToBase64Uploader.generated.h"


UCLASS()
class POSTPRODUCTION_API UFileToBase64Uploader : public UObject
{
	GENERATED_BODY()
	
public:
	UFileToBase64Uploader();

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
