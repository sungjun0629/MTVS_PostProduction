// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseFilesDownloader_Plugin.h"
#include "FileToStorageDownloader_Plugin.generated.h"

/** Possible results from a download request */
UENUM(BlueprintType, Category = "File To Storage Downloader")
enum class EDownloadToStorageResult_Plugin : uint8
{
	Success,
	/** Downloaded successfully, but there was no Content-Length header in the response and thus downloaded by payload */
	SucceededByPayload,
	Cancelled,
	DownloadFailed,
	SaveFailed,
	DirectoryCreationFailed,
	InvalidURL,
	InvalidSavePath
};

/** Static delegate broadcast after the download is complete */
DECLARE_DELEGATE_OneParam(FOnFileToStorageDownloadCompleteNative, EDownloadToStorageResult_Plugin);

/** Dynamic delegate broadcast after the download is complete */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFileToStorageDownloadComplete, EDownloadToStorageResult_Plugin, Result);

enum class EDownloadToMemoryResult_Plugin : uint8;

 UCLASS(BlueprintType, Category = "Runtime Files Downloader|Memory")
class UFileToStorageDownloader_Plugin : public UBaseFilesDownloader_Plugin
{
	GENERATED_BODY()
public:

protected:
	/** Static delegate for monitoring the completion of the download */
	FOnFileToStorageDownloadCompleteNative OnDownloadComplete;

public:
	/**
	 * Download the file and save it to storage
	 *
	 * @param URL The URL of the file to be downloaded
	 * @param SavePath The absolute path and file name to save the downloaded file
	 * @param Timeout The maximum time to wait for the download to complete, in seconds. Works only for engine versions >= 4.26
	 * @param ContentType A string to set in the Content-Type header field. Use a MIME type to specify the file type
	 * @param bForceByPayload If true, download the file regardless of the Content-Length header's presence (useful for servers without support for this header)
	 * @param OnProgress Delegate for download progress updates
	 * @param OnComplete Delegate for broadcasting the completion of the download
	 */
	UFUNCTION(BlueprintCallable, Category = "Runtime Files Downloader|Storage")
	static UFileToStorageDownloader_Plugin* DownloadFileToStorage(const FString& URL, const FString& SavePath, float Timeout, const FString& ContentType, bool bForceByPayload, const FOnDownloadProgress& OnProgress, const FOnFileToStorageDownloadComplete& OnComplete);

	/**
	 * Download the file and save it to storage. Suitable for use in C++
	 *
	 * @param URL The URL of the file to be downloaded
	 * @param SavePath The absolute path and file name to save the downloaded file
	 * @param Timeout The maximum time to wait for the download to complete, in seconds. Works only for engine versions >= 4.26
	 * @param ContentType A string to set in the Content-Type header field. Use a MIME type to specify the file type
	 * @param bForceByPayload If true, download the file regardless of the Content-Length header's presence (useful for servers without support for this header)
	 * @param OnProgress Delegate for download progress updates
	 * @param OnComplete Delegate for broadcasting the completion of the download
	 */
	static UFileToStorageDownloader_Plugin* DownloadFileToStorage(const FString& URL, const FString& SavePath, float Timeout, const FString& ContentType, bool bForceByPayload, const FOnDownloadProgressNative& OnProgress, const FOnFileToStorageDownloadCompleteNative& OnComplete);

	//~ Begin UBaseFilesDownloader Interface
	virtual bool CancelDownload() override;
	//~ End UBaseFilesDownloader Interface

protected:
	/**
	 * Download the file and save it to the device disk
	 *
	 * @param URL The file URL to be downloaded
	 * @param SavePath The absolute path and file name to save the downloaded file
	 * @param Timeout The maximum time to wait for the download to complete, in seconds. Works only for engine versions >= 4.26
	 * @param ContentType A string to set in the Content-Type header field. Use a MIME type to specify the file type
	 * @param bForceByPayload If true, the file will be downloaded by payload even if the Content-Length header is present in the response
	 */
	void DownloadFileToStorage(const FString& URL, const FString& SavePath, float Timeout, const FString& ContentType, bool bForceByPayload);

	/**
	 * Internal callback for when file downloading has finished
	 */
	void OnComplete_Internal(EDownloadToMemoryResult_Plugin Result, TArray64<uint8> DownloadedContent);

protected:
	/** The destination path to save the downloaded file */
	FString FileSavePath;
};
