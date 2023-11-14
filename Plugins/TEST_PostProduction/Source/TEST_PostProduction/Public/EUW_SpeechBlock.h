// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EUW_SpeechBlock.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TEST_POSTPRODUCTION_API UEUW_SpeechBlock : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
		FString aiVoicePath;

	void CallSetAIVoicePath(const FString& path);

	UFUNCTION(BlueprintImplementableEvent)
		void SetAIVoicePath();

};
