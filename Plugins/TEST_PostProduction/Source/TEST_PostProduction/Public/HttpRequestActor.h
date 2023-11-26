// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonParseLibrary_Plugin.h"
#include "Engine/Texture2D.h"
#include "ProjectTable.h"
#include "HttpRequestActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetAllProjectDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetParticularProjectDelegate, FProjectInfo, projectInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetImageTextureDelegate);

USTRUCT(BlueprintType)
struct FWorkerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString email = TEXT("");
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
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
	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnGetParticularProjectDelegate OnGetParticularProjectDelegate;

	TArray<FProjectTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* realTex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FProjectUnit> projectArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProjectInfo projectInfoArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FWorkerInfo workerInfo;

	UFUNCTION(BlueprintCallable)
	void PostProjectRequest(const FString ProjectName, const FString scriptPath , const FString ProjectDes, TArray<FWorkerInfo> StaffInfo, const FString ImagePath);
	UFUNCTION(BlueprintCallable)
	void GetAllProject();
	UFUNCTION(BlueprintCallable)
	void GetParticularProject(int32 number);
	UFUNCTION(BlueprintCallable)
	FSlateBrush GetImageTexture(int32 projectID);


	void OnReciveAllProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnReciveParticularProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnGetImageTexture(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnPostProjectInfo(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);


public:
	UFUNCTION(BlueprintCallable)	
	FString OnImportButtonClicked();
	UFUNCTION(BlueprintCallable)	
	UTexture2D* Base64ToImage(FString Base64);
};
