// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFilesDownloader_Plugin.h"
#include "Containers/UnrealString.h"
#include "ImageUtils.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "RuntimeChunkDownloader_Plugin.h"

UBaseFilesDownloader_Plugin::UBaseFilesDownloader_Plugin()
{
	FWorldDelegates::OnWorldCleanup.AddWeakLambda(this, [this](UWorld* World, bool bSessionEnded, bool bCleanupResources)
		{
			if (bSessionEnded)
			{
				CancelDownload();
			}
		});
}

bool UBaseFilesDownloader_Plugin::CancelDownload()
{
	return true;
}

void UBaseFilesDownloader_Plugin::GetContentSize(const FString& URL, float Timeout, const FOnGetDownloadContentLength& OnComplete)
{
	GetContentSize(URL, Timeout, FOnGetDownloadContentLengthNative::CreateLambda([OnComplete](int64 ContentSize)
		{
			OnComplete.ExecuteIfBound(ContentSize);
		}));
}

void UBaseFilesDownloader_Plugin::GetContentSize(const FString& URL, float Timeout, const FOnGetDownloadContentLengthNative& OnComplete)
{
	UBaseFilesDownloader_Plugin* FileDownloader = NewObject<UBaseFilesDownloader_Plugin>();
	FileDownloader->AddToRoot();
	FileDownloader->RuntimeChunkDownloaderPtr = MakeShared<FRuntimeChunkDownloader_Plugin>();
	FileDownloader->RuntimeChunkDownloaderPtr->GetContentSize(URL, Timeout).Next([FileDownloader, OnComplete](int64 ContentSize)
		{
			if (FileDownloader)
			{
				FileDownloader->RemoveFromRoot();
			}
			OnComplete.ExecuteIfBound(ContentSize);
		});
}

FString UBaseFilesDownloader_Plugin::BytesToString(const TArray<uint8>& Bytes)
{
	const uint8* BytesData = Bytes.GetData();
	FString Result;
	for (int32 Count = Bytes.Num(); Count > 0; --Count)
	{
		Result += static_cast<TCHAR>(*BytesData);

		++BytesData;
	}
	return Result;
}

UTexture2D* UBaseFilesDownloader_Plugin::BytesToTexture(const TArray<uint8>& Bytes)
{
	return FImageUtils::ImportBufferAsTexture2D(Bytes);
}

bool UBaseFilesDownloader_Plugin::LoadFileToArray(const FString& Filename, TArray<uint8>& Result)
{
	return FFileHelper::LoadFileToArray(Result, *Filename);
}

bool UBaseFilesDownloader_Plugin::SaveArrayToFile(const TArray<uint8>& Bytes, const FString& Filename)
{
	return FFileHelper::SaveArrayToFile(Bytes, *Filename);
}

bool UBaseFilesDownloader_Plugin::LoadFileToString(FString& Result, const FString& Filename)
{
	return FFileHelper::LoadFileToString(Result, *Filename);
}

bool UBaseFilesDownloader_Plugin::SaveStringToFile(const FString& String, const FString& Filename)
{
	return FFileHelper::SaveStringToFile(String, *Filename);
}


bool UBaseFilesDownloader_Plugin::IsFileExist(const FString& FilePath)
{
	return FPaths::FileExists(FilePath);
}

void UBaseFilesDownloader_Plugin::BroadcastProgress(int64 BytesReceived, int64 ContentLength, float ProgressRatio) const
{
	if (OnDownloadProgress.IsBound())
	{
		OnDownloadProgress.Execute(BytesReceived, ContentLength, ProgressRatio);
	}
}
