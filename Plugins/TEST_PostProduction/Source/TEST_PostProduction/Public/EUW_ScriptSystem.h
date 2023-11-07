#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Engine/DataTable.h"
#include "EUW_ScriptSystem.generated.h"

USTRUCT(BlueprintType)
struct FScriptDT : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( DisplayName = "Category" ))
	FString Category;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( DisplayName = "Speaker" ))
	FString Speaker;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( DisplayName = "Script" ))
	FString Script;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( DisplayName = "Location" ))
	FString Location;
};

UCLASS(BlueprintType)
class TEST_POSTPRODUCTION_API UEUW_ScriptSystem : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UEUW_ScriptSystem();

	// Script System Components
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = ScriptData)
	class UDataTable* ScriptDataTable;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ScriptData)
	class UButton* B_ImportScript;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ScriptData)
	class UScrollBox* SB_ScriptList;

	// Sound System Components

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundWidget)
	class USlider* Slide_Original;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundWidget)
	class USlider* Slide_Converted;


	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundWidget)
	class UButton* B_LoadVoice;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundWidget)
	class UButton* B_StartVoiceConvert;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundControl)
	class UButton* B_PlayVoice;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundControl)
	class UButton* B_StopVoice;

	// Not using current
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundControl)
	class UButton* B_PreviousButton;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundControl)
	class UButton* B_NextButton;

	// Convert Example
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ConvertSample)
	class UButton* B_1stConvertTarget;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ConvertSample)
	class UButton* B_2ndConvertTarget;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ConvertSample)
	class UButton* B_3rdConvertTarget;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ConvertSample)
	class UButton* B_4thConvertTarget;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ConvertSample)
	class UButton* B_5thConvertTarget;


};
