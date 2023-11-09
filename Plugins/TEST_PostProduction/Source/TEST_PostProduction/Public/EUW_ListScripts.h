// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Engine/DataTable.h"
#include "EUW_ListScripts.generated.h"

USTRUCT(BlueprintType)
struct FScriptData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Category"))
	FString Category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Speaker"))
	FString Speaker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Script"))
	FString Script;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Location"))
	FString Location;
};

UCLASS(BlueprintType)
class TEST_POSTPRODUCTION_API UEUW_ListScripts : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UEUW_ListScripts();
	
	/*UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Script)
	class UTextBlock* Text_Script;

	UFUNCTION(BlueprintCallable)
	void CheckScript();*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScriptData)
	uint8 ScriptCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ScriptData)
	UDataTable* ScriptDataTable;
};
