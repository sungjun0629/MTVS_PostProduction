// Fill out your copyright notice in the Description page of Project Settings.


#include "ImportExportDataTable.h"
#include "Factories/ReimportDataTableFactory.h"

#include"Editor/UnrealEd/Public/AssetImportTask.h"
#include"AssetToolsModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManagerGeneric.h"

UDataTable* UExcelImportExportDataTable::ImportDataTableFromCSV(FString SourcePath , FString DestinationPath , UScriptStruct* StructClass , bool& bOutSuccess , FString& OutInfoMessage)
{
	//Create factory to import a Data Table
	UReimportDataTableFactory* Factory = NewObject<UReimportDataTableFactory>();
	Factory->AutomatedImportSettings.ImportType = ECSVImportType::ECSV_DataTable;
	Factory->AutomatedImportSettings.ImportRowStruct = StructClass;

	// Crea the import task
	UAssetImportTask* ImportTask = CreateImportTask(SourcePath, DestinationPath, Factory, nullptr, bOutSuccess, OutInfoMessage);
	if ( !bOutSuccess )
	{
		return nullptr;
	}

	// Import the asset
	UObject* ImportedAsset = ProcessImportTask(ImportTask, bOutSuccess, OutInfoMessage);
	if ( !bOutSuccess )
	{
		return nullptr;
	}

	// Return imported asset
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Import success for %s"), *DestinationPath);
	return Cast<UDataTable>(ImportedAsset);
}

void UExcelImportExportDataTable::ExportDataTableToCSV(FString FilePath , UDataTable* DataTable , bool& bOutSuccess , FString& OutInfoMessage)
{
	if ( DataTable == nullptr )
	{
		bOutSuccess = false;
		UE_LOG(LogTemp , Error , TEXT("ExportDataTableToCSV : DataTable is nullptr"));
		return;
	}

	FString TableString;
	UE_LOG(LogTemp , Warning , TEXT("ExportDataTableToCSV"));

	FFileManagerGeneric FileManager;

	TableString = DataTable->GetTableAsCSV();

	WriteStringToFile(FilePath , TableString , bOutSuccess , OutInfoMessage);
	
}

UAssetImportTask* UExcelImportExportDataTable::CreateImportTask(FString SourcePath , FString DestinationPath , UFactory* ExtraFactory , UObject* ExtreaOptions , bool& bOutSuccess , FString& OutInfoMessage)
{
	//Create task Object
	UAssetImportTask * Task = NewObject<UAssetImportTask>();

	if ( Task == nullptr )
	{
		bOutSuccess = false;
		UE_LOG(LogTemp , Error , TEXT("CreateImportTask : UAssetImportTask is nullptr"));
		return nullptr;
	}

	// Set path informations
	Task->Filename = SourcePath;
	Task->DestinationPath = FPaths::GetPath(DestinationPath);
	Task->DestinationName = FPaths::GetCleanFilename(DestinationPath);

	// Set basic options
	Task->bSave = false;
	Task->bAutomated = true;
	Task->bAsync = false;
	Task->bReplaceExisting = true;
	Task->bReplaceExistingSettings = false;

	// Optional extra factory and options
	Task->Factory = ExtraFactory;
	Task->Options = ExtreaOptions;
	
	// Return the task
	bOutSuccess	= true;
	OutInfoMessage = FString::Printf(TEXT("Task created for %s"),*SourcePath);
	return Task;

}
  
UObject* UExcelImportExportDataTable::ProcessImportTask(UAssetImportTask* Task , bool& bOutSuccess , FString& OutInfoMessage)
{
	// Check if the task is valid
	if ( Task == nullptr )
	{
		UE_LOG(LogTemp , Error , TEXT("ProcessImportTask : UAssetImportTask is nullptr"));
		return nullptr;
	}

	 // Get the AssetToolsModule
	 FAssetToolsModule* AssetToolsModule = &FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	 if ( AssetToolsModule == nullptr )
	 {
		bOutSuccess = false;
		 UE_LOG(LogTemp , Error , TEXT("ProcessImportTask : AssetToolsModule is nullptr"));
		 return nullptr;
	 }
	
	AssetToolsModule->Get().ImportAssetTasks({Task});

	if ( Task->GetObjects().Num() == 0 )
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Task failed for %s"),*Task->Filename);
		return	nullptr;
	}

	// Because some import tasks actually create multiple assets, we manually get the right assset
	UObject* ImportedAsset = StaticLoadObject(UObject::StaticClass() , nullptr , *FPaths::Combine(Task->DestinationPath, Task->DestinationName));

	// Return the asset
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Task success for %s"),*Task->Filename);
	return ImportedAsset;
}

UObject* UExcelImportExportDataTable::ImportAsset(FString SourcePath , FString DestinationPath , bool& bOutSuccess , FString& OutInfoMessage)
{
	UAssetImportTask* Task = CreateImportTask(SourcePath , DestinationPath , nullptr , nullptr , bOutSuccess , OutInfoMessage);
	if ( !bOutSuccess )
	{
		return	nullptr;
	}

	UObject* RetAsset = ProcessImportTask(Task , bOutSuccess , OutInfoMessage);
	if ( !bOutSuccess )
	{
		return	nullptr;
	}

	return RetAsset;
}

void UExcelImportExportDataTable::WriteStringToFile(FString FilePath , FString StringToWrite , bool& bOutSuccess , FString& OutInfoMessage)
{
	// Try To write the String into the file
	if ( !FFileHelper::SaveStringToFile(StringToWrite , *FilePath) )
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Failed to write into file %s , %s"),*FilePath, *StringToWrite);
		UE_LOG(LogTemp , Warning , TEXT("Fail to write into file %s"), *OutInfoMessage);
		return;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Success to write into file %s"),*FilePath);
	UE_LOG(LogTemp , Warning , TEXT("Success to write into file %s"), *OutInfoMessage);
}
