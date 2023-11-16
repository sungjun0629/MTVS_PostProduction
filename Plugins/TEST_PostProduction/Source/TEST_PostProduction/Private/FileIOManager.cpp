#include "FileIOManager.h"
#include "AssetExportTask.h"
#include "AssetToolsModule.h"
#include "Exporters/SoundExporterWAV.h"
#include "Runtime/Engine/Classes/Sound/SoundWave.h"
#include "Editor/UnrealEd/Public/AssetImportTask.h"

UAssetImportTask* UFileIOManager::CreateImportTask(FString SourcePath , FString DestinationPath , UFactory* ExtraFactory , UObject* ExtraOptions , bool& bOutSuccess , FString& OutInfoMessage)
{
	UAssetImportTask* RetTask = NewObject<UAssetImportTask>();

	if ( RetTask == nullptr )
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Create Import Task Failed. Have to check Paths : %s"), *SourcePath);
		return nullptr;
	}

	RetTask->Filename = SourcePath;
	RetTask->DestinationPath = FPaths::GetPath(DestinationPath);
	RetTask->DestinationName = FPaths::GetCleanFilename(DestinationPath);

	RetTask->bSave = false;
	RetTask->bAutomated = false;
	RetTask->bAsync = false;
	RetTask->bReplaceExisting = true;
	RetTask->bReplaceExistingSettings = false;

	RetTask->Factory = ExtraFactory;
	RetTask->Options = ExtraOptions;

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Create Import Task Succeeded : %s"), *SourcePath);
	return RetTask;
}

UObject* UFileIOManager::ProcessImportTask(UAssetImportTask* ImportTask , bool& bOutSuccess , FString& OutInfoMessage)
{
	if ( ImportTask == nullptr )
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Process Import Asset Failed. All Task was Invalid"));
		return nullptr;
	}

	FAssetToolsModule* AssetTools = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools");

	if ( AssetTools == nullptr )
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Process Import Asset Failed. The AssetTools Module is Invalid"));
		return nullptr;
	}
	
	AssetTools->Get().ImportAssetTasks({ ImportTask });

	if ( ImportTask->GetObjects().Num() == 0 )
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Process Import Asset Failed. Noting was Imported. Is your file valid?"));
		return nullptr;
	}

	UObject* ImportedAsset = StaticLoadObject(UObject::StaticClass() , nullptr , *FPaths::Combine(ImportTask->DestinationPath , ImportTask->DestinationName));

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Process Import Task Succeeded"));
	return ImportedAsset;
}

UObject* UFileIOManager::ImportAsset(FString SourcePath , FString DestinationPath , bool& bOutSuccess , FString& OutInfoMessage)
{
	UAssetImportTask* Task = CreateImportTask(SourcePath , DestinationPath , nullptr , nullptr , bOutSuccess , OutInfoMessage);
	if ( !bOutSuccess ) { return nullptr; }

	UObject* RetAsset = ProcessImportTask(Task , bOutSuccess , OutInfoMessage);
	if ( !bOutSuccess ) { return nullptr; }

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Import Asset Succeeded"));
	return RetAsset;
}

void UFileIOManager::ExportAsset(UObject* TargetObject , FString ExportPath)
{
	if ( TargetObject == nullptr )
	{
		UE_LOG(LogTemp , Warning , TEXT("Object is nullptr"));
		return;
	}

	FString TargetName = TargetObject->GetName();

	if ( TargetName.Contains("Audio") )
	{
		TArray<uint8> WavData;
		USoundWave* TargetWAV = Cast<USoundWave>(TargetObject);

		const uint8* RawWaveData = TargetWAV->GetResourceData();
		const int32 RawDataSize = TargetWAV->GetResourceSize();

		const FString FilePath = FPaths::ProjectContentDir() + ExportPath + ".wav";
		FFileHelper::SaveArrayToFile(WavData , *FilePath);

		UE_LOG(LogTemp , Warning , TEXT("Export Complete"));

		/*UAssetExportTask* Task = NewObject<UAssetExportTask>();
		Task->bAutomated = true;
		Task->Filename = TEXT("%s", *TargetName);
		bool Res = USoundExporterWAV::RunAssetExportTask(Task);*/
	}
}

