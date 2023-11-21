// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FileToStorageDownloader_Plugin.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "MemoTableEditor.h"
#include "SGetWebAddress.h"
#include "Templates/SharedPointer.h"
#include "SoundConverterLogic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDownloadSuccessBlueprint, FString, savepath);
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

	UPROPERTY(BlueprintAssignable, Category = "SoundConverter")
	FDownloadSuccessBlueprint OnDownloadSuccessBlueprint;

	UFUNCTION(BlueprintCallable, Category = "SoundConverter")
	void ConvertedSoundDownload(FString loadedAsset, FString modelName);

	UFUNCTION(BlueprintCallable, Category = "SoundConverter")
	void SuccessDownload(EDownloadToStorageResult_Plugin Result);

	void OnDownloadConvertedVoice(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);

	void DownloadVoice(FString url);

	UFUNCTION(BlueprintImplementableEvent, Category = "SoundConverter")
	void OnDownloadComplete();

	/*UFUNCTION(BlueprintCallable, Category = "SoundConverter")
	FString GetDownloadedVoicePath();*/
	
	UPROPERTY(BlueprintReadWrite, Category = "SoundConverter")
	FString downloadedVoicePath;

	FString SavePath;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString ConvertedFileName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OriginalFilePath;

	class UEUW_SpeechBlock* speechBlock;

	FSlateBrush MySlateBrush;
	// 이미지 테스트용
	const FSlateBrush* SearchImageFromUE(FString imagePath);

	void GetSequenceAsset();
	TArray<TSharedPtr<FString>> Options;

	TSharedPtr<SGetWebAddress> getWebAddress;

	// 테스트 코드
	TSharedPtr<FMemoTableEditor , ESPMode::ThreadSafe> MemoTableEditorInstance = MakeShared<FMemoTableEditor>();
};
