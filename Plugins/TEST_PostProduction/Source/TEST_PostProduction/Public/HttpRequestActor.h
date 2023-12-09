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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetParticularSceneCardDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetParticularProjectDelegate, FProjectInfo, projectInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetImageTextureDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostSuccessDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScriptSuccessDelegate);

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
	UHttpRequestActor();
	~UHttpRequestActor();
	

	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnGetAllProjectDelegate OnreciveAllProjectDeletage; 
	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnGetImageTextureDelegate OnGetImageTextureDelegate;
	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnGetParticularProjectDelegate OnGetParticularProjectDelegate;
	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnPostSuccessDelegate OnSuccessPostDelegate;

	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnGetParticularSceneCardDelegate OnGetParticularSceneCardDelegate;
	UPROPERTY(BlueprintAssignable, Category = "PPHttp")
	FOnScriptSuccessDelegate OnScriptSuccessDelegate;

	FOnDownloadProgress OnDownloadProgressDelegate;
	FOnFileToStorageDownloadComplete OnFileToStorageDownloadCompleteDelegate;


	TArray<FProjectTable*> TableRows; // Assuming FMyDataTableType is the struct type of your DataTable rows.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* realTex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FProjectUnit> projectArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProjectInfo projectInfoArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FWorkerInfo workerInfo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FSceneCardInfo sceneInfo;

	UFUNCTION(BlueprintCallable)
	void PostProjectRequest(const FString ProjectName, const FString scriptPath , const FString ProjectDes, TArray<FWorkerInfo> StaffInfo, const FString ImagePath);
	UFUNCTION(BlueprintCallable)
	void PostSceneCard(int32 projectID , int32 sceneNo ,const FString story, FString levelLocation, FString imagePath);
	UFUNCTION(BlueprintCallable)
	void GetAllProject();
	UFUNCTION(BlueprintCallable)
	void GetScriptCSV(int32 projectId);
	UFUNCTION(BlueprintCallable)
	void GetParticularProject(int32 number);
	UFUNCTION(BlueprintCallable)
	void GetParticularSceneCard(int32 projectID, int32 sceneNo);
	UFUNCTION(BlueprintCallable)
	FSlateBrush GetImageTexture(int32 projectID);

	void GetCSVDownload();
	UFUNCTION()
	void SuccessCSVDownload(EDownloadToStorageResult_Plugin Result);
	void PostCSVToStorage(FString savePath);

	void OnReciveAllProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnReciveParticularProject(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnGetImageTexture(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnPostProjectInfo(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnPostSceneCardInfo(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnPostCSVFile(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnGetCSVDownloadURL(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnGetScriptCSVDownload(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnGetParticularSceneCard(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);

	bool isScript = false;

public:
	UFUNCTION(BlueprintCallable)	
	FString OnImportButtonClicked();
	UFUNCTION(BlueprintCallable)	
	UTexture2D* Base64ToImage(FString Base64);
	UFUNCTION(BlueprintCallable)
	FString ConvertToContentPath(const FString& FullPath);
};
