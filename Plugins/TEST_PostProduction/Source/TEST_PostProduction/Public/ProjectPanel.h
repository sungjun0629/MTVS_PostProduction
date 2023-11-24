// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ProjectPanel.generated.h"

/**
 * 
 */
UCLASS()
class TEST_POSTPRODUCTION_API UProjectPanel : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
	public:
	virtual void NativeConstruct() override;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UEditableText* edit_page;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UEditableText* edit_row;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	class UButton* btn_send;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	class UButton* btn_post;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	class UTextBlock* T_TestBlock;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UTextBlock* text_log;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UImage* img_Http;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UButton* btn_image;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UButton* btn_imagePost;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UButton* btn_csvSingle;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UButton* btn_csvAll;

	//UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = MySettings)
	//class UButton* btn_csvFile;


private:
	UFUNCTION()
	void SendRequest();

	UFUNCTION()
	void PostRequest();

	//UFUNCTION()
	//void GetImageRequest();

	//UFUNCTION()
	//void PostImageRequest();

	//UFUNCTION()
	//void ReadCsvSingle();

	//UFUNCTION()
	//void ReadCsvAll();

	//UFUNCTION()
	//void ReadCsvFile();

	UPROPERTY()
	class AHttpRequestActor* httpReqActor;

	UPROPERTY(EditAnywhere, Category=MySettings)
	class UDataTable* playerInfoTable;

	FString baseURL = "http://apis.data.go.kr/4050000/libnewbk/getLibnewbk";
	FString baseURL2 = "http://127.0.0.1:5000";
	FString decodingKey = "OGGE4bF0qIWocddXNy1FegaoUiuA/3FcrT8NwSLiBEx05DYzelJFJTmyJETsV4GmbGIhFuEncU6qMzPMTihraA==";


	// PostProject
	FString projectAllUrl = "http://121.165.108.236:4492/group/project/all";

	// base + number
	FString projectDetailUrlBase = "http://121.165.108.236:4492/group/project/";
};
