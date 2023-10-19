#include "BaseFilesDownloader.h"

UBaseFilesDownloader::UBaseFilesDownloader()
{
}

bool UBaseFilesDownloader::CancelDownload()
{
	return false;
}

void UBaseFilesDownloader::GetContentSize(const FString& URL, float Timeout, const FOnGetDownloadContentLength& OnComplete)
{
}

void UBaseFilesDownloader::GetContentSize(const FString& URL, float Timeout, const FOnGetDownloadContentLengthNative& OnComplete)
{
}

FString UBaseFilesDownloader::BytesToString(const TArray<uint8>& Bytes)
{
	return FString();
}

UTexture2D* UBaseFilesDownloader::BytesToTexture(const TArray<uint8>& Bytes)
{
	return nullptr;
}

bool UBaseFilesDownloader::LoadFileToArray(const FString& FilePath, TArray<uint8>& Result)
{
	return false;
}

bool UBaseFilesDownloader::SaveArrayToFile(const TArray<uint8>& Bytes, const FString& FilePath)
{
	return false;
}

bool UBaseFilesDownloader::LoadFileToString(FString& Result, const FString& FilePath)
{
	return false;
}

bool UBaseFilesDownloader::SaveStringToFile(const FString& String, const FString& FilePath)
{
	return false;
}

bool UBaseFilesDownloader::IsFileExist(const FString& FilePath)
{
	return false;
}

void UBaseFilesDownloader::BroadcastProgress(int64 BytesReceived, int64 ContentLength, float ProgressRatio) const
{
}
