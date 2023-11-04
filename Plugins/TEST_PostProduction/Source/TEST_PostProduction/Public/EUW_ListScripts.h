// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "EUW_ListScripts.generated.h"


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
};
