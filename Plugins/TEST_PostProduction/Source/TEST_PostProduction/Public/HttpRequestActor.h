// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonParseLibrary_Plugin.h"
#include "HttpRequestActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetAllProjectDelegate);

USTRUCT(BlueprintType)
struct FWorkerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FString email = TEXT("");
	FString role = TEXT("");

	FORCEINLINE void SetInfo(FString uID, FString uName) { email = uID ; role = uName; }
};


UCLASS(BlueprintType, Category = "PPHttp")
class UHttpRequestActor : public UObject
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnGetAllProjectDelegate OnreciveAllProjectDeletage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FProjectUnit> projectArray;

	void SendRequest(const FString url);
	void PostRequest(const FString url);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FWorkerInfo workerInfo;

	UFUNCTION(BlueprintCallable)
	void PostProjectRequest(const FString ProjectName, const FString ProjectDes, TArray<FWorkerInfo> StaffInfo, const FString ImageBase64);
	UFUNCTION(BlueprintCallable)
	void GetAllProject();
	UFUNCTION(BlueprintCallable)
	void GetParticularProject(int32 number);

	void OnReciveAllProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnReciveParticularProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	//void SaveJson(const FString jsonData);
	//void GetImage(const FString url);
	//void SaveImage(const UTexture2D* tex);
	//void PostImage(const FString url , const UTexture2D* tex);

private:
	void OnReceivedData(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnPostData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);
	//void OnGetImageData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);
	//void OnPostTextureData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);


};
