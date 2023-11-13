// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EUW_SessionThumbnail.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TEST_POSTPRODUCTION_API UEUW_SessionThumbnail : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	UEUW_SessionThumbnail(){}
	UEUW_SessionThumbnail(FString SceneName , FString Description , UTexture2D* thumbnailSource);

	// UI Element
	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ScriptData)
	class UTextBlock* T_SceneName;

	UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ScriptData)
	class UTextBlock* T_Describtion;
	
	//UPROPERTY(EditAnywhere , BlueprintReadWrite , meta = ( BindWidget ) , Category = ScriptData)
	//class UImage* I_Thumbnail;


	// Source
	class UTexture2D* YourTexture; // Your image texture

	UFUNCTION(BlueprintCallable)
	void SetThumbnailInfo(FString SceneName , FString Description , UTexture2D* thumbnailSource);
};
