#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Engine/DataTable.h"
#include "UW_SingleScript.h"
#include "EUW_ScriptSystem.generated.h"

USTRUCT(BlueprintType)
struct FScriptDT : public FTableRowBase
{
	GENERATED_BODY()
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UUW_SingleScript> SingleScriptClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	class UUW_SingleScript* SingleScriptWidget;
	
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

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundControl)
	class UButton* B_Original;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = SoundControl)
	class UButton* B_Converted;

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

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = SoundData)
	class UAudioComponent* AudioComp;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = SoundData)
	class USoundBase* UsingSound;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = SoundData)
	class USoundBase* OriginalSoundBase;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = SoundData)
	class USoundBase* ConvertedSoundBase;
	
	float CurrentPercent = 0;
	float CurrentPlayTime = 0;
	bool bIsPlayingOriginal = true;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void BtnOnClicked_ImportScript();
	
	UFUNCTION()
	void BtnOnClicked_PlayVoice();

	UFUNCTION()
	void BtnOnClicked_StopVoice();

	UFUNCTION()
	void BtnOnClicked_ChooseOriginal();

	UFUNCTION()
	void BtnOnClicked_ChooseConverted();

	UFUNCTION()
	void OnAudioPlayBackPercent_Event(const class USoundWave* PlayingSoundWave, const float PlaybackPercent);

	UFUNCTION()
	void OnAudioPlayBackPercentChanged_Event(const class USoundWave* PlayingSoundWave , const float PlaybackPercent);

	UFUNCTION()
	void SlideBeginCapture_Original();

	UFUNCTION()
	void SlideBeginCapture_Converted();

	UFUNCTION()
	void SlideEndCapture_Original();

	UFUNCTION()
	void SlideEndCapture_Converted();

	UFUNCTION()
	void SlideValueChanged_Original(float Value);

	UFUNCTION()
	void SlideValueChanged_Converted(float Value);



};
