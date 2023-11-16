// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FileIOManager.generated.h"

UCLASS()
class TEST_POSTPRODUCTION_API UFileIOManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	static UAssetImportTask* CreateImportTask(FString SourcePath , FString DestinationPath , UFactory* ExtraFactory , UObject* ExtraOptions , bool& bOutSuccess , FString& OutInfoMessage);

	UFUNCTION()
	static UObject* ProcessImportTask(UAssetImportTask* ImportTask , bool& bOutSuccess , FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable)
	static UObject* ImportAsset(FString SourcePath , FString DestinationPath , bool& bOutSuccess , FString& OutInfoMessage); 

	UFUNCTION(BlueprintCallable)
	static void ExportAsset(UObject* TargetObject, FString ExportPath);
};
