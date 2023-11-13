// Fill out your copyright notice in the Description page of Project Settings.


#include "EUW_SessionThumbnail.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UEUW_SessionThumbnail::UEUW_SessionThumbnail(FString sceneName , FString description , UTexture2D* thumbnailSource)
{
	SetThumbnailInfo(sceneName, description, thumbnailSource);
}

void UEUW_SessionThumbnail::SetThumbnailInfo(FString sceneName, FString description, UTexture2D* thumbnailSource)
{
	// Title 과 설명 추가
	T_SceneName->SetText(FText::FromString(sceneName));
	T_Describtion->SetText(FText::FromString(description));
	

	//YourTexture = LoadObject<UTexture2D>(nullptr , TEXT("YourTexturePath"));


	//if ( thumbnailSource )
	//{
	//	// Set the image
	//	I_Thumbnail->SetBrushFromTexture(YourTexture);
	//}
}

