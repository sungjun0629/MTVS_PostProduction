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
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Script)
	class UTextBlock* T_Category;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = ( BindWidget ) , Category = Script)
	class UTextBlock* T_Location;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = ( BindWidget ) , Category = Script)
	class UTextBlock* T_SceneNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = ( BindWidget ) , Category = Script)
	class UTextBlock* T_Script;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = ( BindWidget ) , Category = Script)
	class UTextBlock* T_Speaker;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Category;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString Speaker;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString Script;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString Location;
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	FString SceneNumber;
	
	UFUNCTION(BlueprintCallable)
	void GetScriptTable(FString WriteCategory , FString WriteSpeaker , FString WriteScript , FString WriteLocation);

	UFUNCTION(BlueprintCallable)
	void SetTextToUI(UTextBlock* TextBlock, FString Data);

	UFUNCTION(BlueprintCallable)
	void GetScriptDataFromSystem(FString SystemCategory, FString SystemSpeaker, FString SystemScript, FString SystemLocation, FString SystemSceneNum);

	void IsSceneNumber(FString SystemSpeaker, FString SystemLocation);
	void IsNotSceneNumber(FString SystemSceneNum, FString SystemSpeaker, FString SystemScript, FString SystemLocation);
};
