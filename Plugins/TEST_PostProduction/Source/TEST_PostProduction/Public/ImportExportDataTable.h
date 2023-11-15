#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//#include "ExcelImportExportDataTable.generated.h"

class UDataTable;
class UAssetImportTask;
class UFactory;

/**
 * 
 */
//UCLASS()
class UExcelImportExportDataTable : public UBlueprintFunctionLibrary
{
public:

UFUNCTION(BlueprintCallable)
	static UDataTable* ImportDataTableFromCSV(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass, bool& bOutSuccess, FString& OutInfoMessage);

UFUNCTION(BlueprintCallable)
	static void ExportDataTableToCSV(FString FilePath, UDataTable* DataTable, bool& bOutSuccess, FString& OutInfoMessage);

UFUNCTION(BlueprintCallable)
	static UAssetImportTask* CreateImportTask(FString SourcePath, FString DestinationPath, UFactory* ExtraFactory, UObject* ExtreaOptions, bool& bOutSuccess, FString& OutInfoMessage);

UFUNCTION(BlueprintCallable)
	static UObject* ProcessImportTask(UAssetImportTask* Task, bool& bOutSuccess, FString& OutInfoMessage);

UFUNCTION(BlueprintCallable)
	static UObject* ImportAsset(FString SourcePath, FString DestinationPath, bool& bOutSuccess, FString& OutInfoMessage);

UFUNCTION(BlueprintCallable)
	static void WriteStringToFile(FString FilePath, FString StringToWrite, bool& bOutSuccess, FString& OutInfoMessage);

};
