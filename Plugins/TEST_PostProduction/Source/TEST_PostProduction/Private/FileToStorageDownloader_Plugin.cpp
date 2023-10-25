// Fill out your copyright notice in the Description page of Project Settings.


#include "FileToStorageDownloader_Plugin.h"
#include "RuntimeChunkDownloader_Plugin.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformFileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"


UFileToStorageDownloader_Plugin* UFileToStorageDownloader_Plugin::DownloadFileToStorage(const FString& URL, const FString& SavePath, float Timeout, const FString& ContentType, bool bForceByPayload, const FOnDownloadProgress& OnProgress, const FOnFileToStorageDownloadComplete& OnComplete)
{
	return DownloadFileToStorage(URL, SavePath, Timeout, ContentType, bForceByPayload, FOnDownloadProgressNative::CreateLambda([OnProgress](int64 BytesReceived, int64 ContentSize, float ProgressRatio)
		{
			OnProgress.ExecuteIfBound(BytesReceived, ContentSize, ProgressRatio);
		}), FOnFileToStorageDownloadCompleteNative::CreateLambda([OnComplete](EDownloadToStorageResult_Plugin Result)
			{
				OnComplete.ExecuteIfBound(Result);
			}));
}

UFileToStorageDownloader_Plugin* UFileToStorageDownloader_Plugin::DownloadFileToStorage(const FString& URL, const FString& SavePath, float Timeout, const FString& ContentType, bool bForceByPayload, const FOnDownloadProgressNative& OnProgress, const FOnFileToStorageDownloadCompleteNative& OnComplete)
{
	UFileToStorageDownloader_Plugin* Downloader = NewObject<UFileToStorageDownloader_Plugin>(StaticClass());
	Downloader->AddToRoot();
	Downloader->OnDownloadProgress = OnProgress;
	Downloader->OnDownloadComplete = OnComplete;
	Downloader->DownloadFileToStorage(URL, SavePath, Timeout, ContentType, bForceByPayload);
	return Downloader;
}

bool UFileToStorageDownloader_Plugin::CancelDownload()
{
	if (RuntimeChunkDownloaderPtr.IsValid())
	{
		RuntimeChunkDownloaderPtr->CancelDownload();
		return true;
	}
	return false;
}

void UFileToStorageDownloader_Plugin::DownloadFileToStorage(const FString& URL, const FString& SavePath, float Timeout, const FString& ContentType, bool bForceByPayload)
{
	if (URL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("You have not provided an URL to download the file"));
		OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::InvalidURL);
		RemoveFromRoot();
		return;
	}

	if (SavePath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("You have not provided a path to save the file"));
		OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::InvalidSavePath);
		RemoveFromRoot();
		return;
	}

	if (Timeout < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The specified timeout (%f) is less than 0, setting it to 0"), Timeout);
		Timeout = 0;
	}

	auto OnProgress = [this](int64 BytesReceived, int64 ContentSize)
		{
			BroadcastProgress(BytesReceived, ContentSize, ContentSize <= 0 ? 0 : static_cast<float>(BytesReceived) / ContentSize);
		};

	auto OnResult = [this](FRuntimeChunkDownloaderResult&& Result) mutable
		{
			OnComplete_Internal(Result.Result, MoveTemp(Result.Data));
		};

	FileSavePath = SavePath;
	RuntimeChunkDownloaderPtr = MakeShared<FRuntimeChunkDownloader_Plugin>();

	if (bForceByPayload)
	{
		RuntimeChunkDownloaderPtr->DownloadFileByPayload(URL, Timeout, ContentType, OnProgress).Next(OnResult);
	}
	else
	{
		RuntimeChunkDownloaderPtr->DownloadFile(URL, Timeout, ContentType, TNumericLimits<TArray<uint8>::SizeType>::Max(), OnProgress).Next(OnResult);
	}
}

void UFileToStorageDownloader_Plugin::OnComplete_Internal(EDownloadToMemoryResult_Plugin Result, TArray64<uint8> DownloadedContent)
{
	RemoveFromRoot();

	if (Result != EDownloadToMemoryResult_Plugin::Success && Result != EDownloadToMemoryResult_Plugin::SucceededByPayload)
	{
		// TODO: redesign in a more elegant way
		switch (Result)
		{
		case EDownloadToMemoryResult_Plugin::Cancelled:
			OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::Cancelled);
			break;
		case EDownloadToMemoryResult_Plugin::DownloadFailed:
			OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::DownloadFailed);
			break;
		case EDownloadToMemoryResult_Plugin::InvalidURL:
			OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::InvalidURL);
			break;
		}
		return;
	}

	if (!DownloadedContent.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Error, TEXT("An error occurred while downloading the file to storage"));
		OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::DownloadFailed);
		return;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Create save directory if it does not exist
	{
		FString Path, Filename, Extension;
		FPaths::Split(FileSavePath, Path, Filename, Extension);
		if (!PlatformFile.DirectoryExists(*Path))
		{
			if (!PlatformFile.CreateDirectoryTree(*Path))
			{
				UE_LOG(LogTemp, Error, TEXT("Unable to create a directory '%s' to save the downloaded file"), *Path);
				OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::DirectoryCreationFailed);
				return;
			}
		}
	}

	// Delete the file if it already exists
	if (FPaths::FileExists(*FileSavePath))
	{
		IFileManager& FileManager = IFileManager::Get();
		if (!FileManager.Delete(*FileSavePath))
		{
			UE_LOG(LogTemp, Error, TEXT("Something went wrong while deleting the existing file '%s'"), *FileSavePath);
			OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::SaveFailed);
			return;
		}
	}

	IFileHandle* FileHandle = PlatformFile.OpenWrite(*FileSavePath);
	if (!FileHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Something went wrong while saving the file '%s'"), *FileSavePath);
		OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::SaveFailed);
		return;
	}

	if (!FileHandle->Write(DownloadedContent.GetData(), DownloadedContent.Num()))
	{
		UE_LOG(LogTemp, Error, TEXT("Something went wrong while writing the response data to the file '%s'"), *FileSavePath);
		delete FileHandle;
		OnDownloadComplete.ExecuteIfBound(EDownloadToStorageResult_Plugin::SaveFailed);
		return;
	}

	delete FileHandle;
	OnDownloadComplete.ExecuteIfBound(Result == EDownloadToMemoryResult_Plugin::SucceededByPayload ? EDownloadToStorageResult_Plugin::SucceededByPayload : EDownloadToStorageResult_Plugin::Success);
}