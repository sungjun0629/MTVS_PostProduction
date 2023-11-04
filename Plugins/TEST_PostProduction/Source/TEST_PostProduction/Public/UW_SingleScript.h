// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_SingleScript.generated.h"

UCLASS(BlueprintType)
class TEST_POSTPRODUCTION_API UUW_SingleScript : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Script)
	class UTextBlock* Text_Script;

	UFUNCTION(BlueprintCallable)
	void CheckScript();
};
