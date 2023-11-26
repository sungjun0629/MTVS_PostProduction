// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonParseLibrary_Plugin.h"
#include "Engine/Texture2D.h"
#include "HttpRequestActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetAllProjectDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetImageTextureDelegate);

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
	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnGetImageTextureDelegate OnGetImageTextureDelegate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* realTex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FProjectUnit> projectArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FWorkerInfo workerInfo;

	UFUNCTION(BlueprintCallable)
	void PostProjectRequest(const FString ProjectName, const FString ProjectDes, TArray<FWorkerInfo> StaffInfo, const FString ImageBase64);
	UFUNCTION(BlueprintCallable)
	void GetAllProject();
	UFUNCTION(BlueprintCallable)
	void GetParticularProject(int32 number);
	UFUNCTION(BlueprintCallable)
	void GetImageTexture(FString url);


	void OnReciveAllProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnReciveParticularProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnGetImageTexture(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);



};
