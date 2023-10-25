// Fill out your copyright notice in the Description page of Project Settings.


#include "FileToMemoryDownloader_Plugin.h"
#include "RuntimeChunkDownloader_Plugin.h"


UFileToMemoryDownloader_Plugin* UFileToMemoryDownloader_Plugin::DownloadFileToMemoryPerChunk(const FString& URL, float Timeout, const FString& ContentType, int32 MaxChunkSize, const FOnDownloadProgress& OnProgress, const FOnFileToMemoryChunkDownloadComplete& OnChunkComplete, const FOnFileToMemoryAllChunksDownloadComplete& OnAllChunksDownloadComplete)
{
	return DownloadFileToMemoryPerChunk(URL, Timeout, ContentType, MaxChunkSize, FOnDownloadProgressNative::CreateLambda([OnProgress](int64 BytesReceived, int64 ContentSize, float Progress)
		{
			OnProgress.ExecuteIfBound(BytesReceived, ContentSize, Progress);
		}), FOnFileToMemoryChunkDownloadCompleteNative::CreateLambda([OnChunkComplete](const TArray64<uint8>& DownloadedContent)
			{
				if (DownloadedContent.Num() > TNumericLimits<int32>::Max())
				{
					/*UE_LOG(LogRuntimeFilesDownloader, Error, TEXT("The size of the downloaded content exceeds the maximum limit for an int32 array. Maximum length: %d, Retrieved length: %lld\nA standard byte array can hold a maximum of 2 GB of data. If you need to download more than 2 GB of data into memory, consider using the C++ native equivalent instead of the Blueprint dynamic delegate"), TNumericLimits<int32>::Max(), DownloadedContent.Num());*/
					OnChunkComplete.ExecuteIfBound(TArray<uint8>());
					return;
				}
				OnChunkComplete.ExecuteIfBound(TArray<uint8>(DownloadedContent));
			}), FOnFileToMemoryAllChunksDownloadCompleteNative::CreateLambda([OnAllChunksDownloadComplete](EDownloadToMemoryResult_Plugin Result)
				{
					OnAllChunksDownloadComplete.ExecuteIfBound(Result);
				}));
}

UFileToMemoryDownloader_Plugin* UFileToMemoryDownloader_Plugin::DownloadFileToMemoryPerChunk(const FString& URL, float Timeout, const FString& ContentType, int64 MaxChunkSize, const FOnDownloadProgressNative& OnProgress, const FOnFileToMemoryChunkDownloadCompleteNative& OnChunkComplete, const FOnFileToMemoryAllChunksDownloadCompleteNative& OnAllChunksDownloadComplete)
{
	UFileToMemoryDownloader_Plugin* Downloader = NewObject<UFileToMemoryDownloader_Plugin>(StaticClass());
	Downloader->AddToRoot();
	Downloader->OnDownloadProgress = OnProgress;
	Downloader->OnChunkDownloadComplete = OnChunkComplete;
	Downloader->OnAllChunksDownloadComplete = OnAllChunksDownloadComplete;
	Downloader->DownloadFileToMemoryPerChunk(URL, Timeout, ContentType, MaxChunkSize);
	return Downloader;
}

UFileToMemoryDownloader_Plugin* UFileToMemoryDownloader_Plugin::DownloadFileToMemory(const FString& URL, float Timeout, const FString& ContentType, bool bForceByPayload, const FOnDownloadProgress& OnProgress, const FOnFileToMemoryDownloadComplete& OnComplete)
{
	return DownloadFileToMemory(URL, Timeout, ContentType, bForceByPayload, FOnDownloadProgressNative::CreateLambda([OnProgress](int64 BytesReceived, int64 ContentSize, float Progress)
		{
			OnProgress.ExecuteIfBound(BytesReceived, ContentSize, Progress);
		}), FOnFileToMemoryDownloadCompleteNative::CreateLambda([OnComplete](const TArray64<uint8>& DownloadedContent, EDownloadToMemoryResult_Plugin Result)
			{
				if (DownloadedContent.Num() > TNumericLimits<int32>::Max())
				{
					/*UE_LOG(LogRuntimeFilesDownloader, Error, TEXT("The size of the downloaded content exceeds the maximum limit for an int32 array. Maximum length: %d, Retrieved length: %lld\nA standard byte array can hold a maximum of 2 GB of data. If you need to download more than 2 GB of data into memory, consider using the C++ native equivalent instead of the Blueprint dynamic delegate"), TNumericLimits<int32>::Max(), DownloadedContent.Num());
					OnComplete.ExecuteIfBound(TArray<uint8>(), EDownloadToMemoryResult_Plugin::DownloadFailed);*/
					return;
				}
				OnComplete.ExecuteIfBound(TArray<uint8>(DownloadedContent), Result);
			}));
}

UFileToMemoryDownloader_Plugin* UFileToMemoryDownloader_Plugin::DownloadFileToMemory(const FString& URL, float Timeout, const FString& ContentType, bool bForceByPayload, const FOnDownloadProgressNative& OnProgress, const FOnFileToMemoryDownloadCompleteNative& OnComplete)
{
	UFileToMemoryDownloader_Plugin* Downloader = NewObject<UFileToMemoryDownloader_Plugin>(StaticClass());
	Downloader->AddToRoot();
	Downloader->OnDownloadProgress = OnProgress;
	Downloader->OnDownloadComplete = OnComplete;
	Downloader->DownloadFileToMemory(URL, Timeout, ContentType, bForceByPayload);
	return Downloader;
}

bool UFileToMemoryDownloader_Plugin::CancelDownload()
{
	if (RuntimeChunkDownloaderPtr.IsValid())
	{
		RuntimeChunkDownloaderPtr->CancelDownload();
		return true;
	}
	return false;
}

void UFileToMemoryDownloader_Plugin::DownloadFileToMemory(const FString& URL, float Timeout, const FString& ContentType, bool bForceByPayload)
{
	if (URL.IsEmpty())
	{
		//UE_LOG(LogRuntimeFilesDownloader, Error, TEXT("You have not provided an URL to download the file"));
		OnDownloadComplete.ExecuteIfBound(TArray64<uint8>(), EDownloadToMemoryResult_Plugin::InvalidURL);
		RemoveFromRoot();
		return;
	}

	if (Timeout < 0)
	{
		//UE_LOG(LogRuntimeFilesDownloader, Warning, TEXT("The specified timeout (%f) is less than 0, setting it to 0"), Timeout);
		Timeout = 0;
	}

	auto OnProgress = [this](int64 BytesReceived, int64 ContentSize)
		{
			BroadcastProgress(BytesReceived, ContentSize, ContentSize <= 0 ? 0 : static_cast<float>(BytesReceived) / ContentSize);
		};

	auto OnResult = [this](FRuntimeChunkDownloaderResult&& Result) mutable
		{
			RemoveFromRoot();
			OnDownloadComplete.ExecuteIfBound(Result.Data, Result.Result);
		};

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

void UFileToMemoryDownloader_Plugin::DownloadFileToMemoryPerChunk(const FString& URL, float Timeout, const FString& ContentType, int64 MaxChunkSize)
{
	if (URL.IsEmpty())
	{
		//UE_LOG(LogRuntimeFilesDownloader, Error, TEXT("You have not provided an URL to download the file"));
		OnDownloadComplete.ExecuteIfBound(TArray64<uint8>(), EDownloadToMemoryResult_Plugin::InvalidURL);
		RemoveFromRoot();
		return;
	}

	if (Timeout < 0)
	{
		//UE_LOG(LogRuntimeFilesDownloader, Warning, TEXT("The specified timeout (%f) is less than 0, setting it to 0"), Timeout);
		Timeout = 0;
	}

	RuntimeChunkDownloaderPtr = MakeShared<FRuntimeChunkDownloader_Plugin>();
	RuntimeChunkDownloaderPtr->DownloadFilePerChunk(URL, Timeout, ContentType, MaxChunkSize, FInt64Vector2(), [this](int64 BytesReceived, int64 ContentSize)
		{
			BroadcastProgress(BytesReceived, ContentSize, ContentSize <= 0 ? 0 : static_cast<float>(BytesReceived) / ContentSize);
		}, [this](TArray64<uint8> DownloadedContent)
			{
				OnChunkDownloadComplete.ExecuteIfBound(DownloadedContent);
			}).Next([this](EDownloadToMemoryResult_Plugin Result)
				{
					RemoveFromRoot();
					OnAllChunksDownloadComplete.ExecuteIfBound(Result);
				});
}