// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPanel.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "EngineUtils.h"
#include "HttpRequestActor.h"
#include "Components/Image.h"
//#include "PlayerInfoTable.h"
//#include "CSVParseLibrary.h"


void UProjectPanel::NativeConstruct()
{
	Super::NativeConstruct();

	btn_send->OnClicked.AddDynamic(this , &UProjectPanel::SendRequest);
	btn_post->OnClicked.AddDynamic(this , &UProjectPanel::PostRequest);
	/*btn_image->OnClicked.AddDynamic(this , &UProjectPanel::GetImageRequest);
	btn_imagePost->OnClicked.AddDynamic(this , &UProjectPanel::PostImageRequest);
	btn_csvSingle->OnClicked.AddDynamic(this , &UProjectPanel::ReadCsvSingle);
	btn_csvAll->OnClicked.AddDynamic(this , &UProjectPanel::ReadCsvAll);
	btn_csvFile->OnClicked.AddDynamic(this , &UProjectPanel::ReadCsvFile);*/

	//httpReqActor = GetWorld()->SpawnActor<AHttpRequestActor>(AHttpRequestActor::StaticClass());
	httpReqActor = NewObject<AHttpRequestActor>();

	if( httpReqActor )UE_LOG(LogTemp , Warning , TEXT("httphttp"));
	//for ( TActorIterator<AHttpRequestActor> it(GetWorld()); it; ++it )
	//{
	//	httpReqActor = *it;
	//}
}

void UProjectPanel::SendRequest()
{
#pragma region Old
	///*if ( !edit_page->GetText().IsEmpty() && !edit_row->GetText().IsEmpty() && httpReqActor != nullptr )
		//{*/
		//	FString fullURL;
		//	/*	fullURL = FString::Printf(TEXT("%s?serviceKey=%s&pageNo=%s&numOfRows=%s") , *baseURL , *decodingKey , *edit_page->GetText().ToString() , *edit_row->GetText().ToString());*/


		//	httpReqActor->SendRequest(fullURL);
		//	UE_LOG(LogTemp , Warning , TEXT("Send Request!"));
		//	UE_LOG(LogTemp , Warning , TEXT("%s") , *fullURL);
		//}
#pragma endregion

	if(httpReqActor!= nullptr)
	{	
		httpReqActor->SendRequest(projectAllUrl);
		UE_LOG(LogTemp , Warning , TEXT("Send Request!"));
		UE_LOG(LogTemp , Warning , TEXT("%s") , *projectAllUrl);
	}
}

void UProjectPanel::PostRequest()
{
	/*if ( httpReqActor != nullptr )
	{*/
		FString myName = TEXT("Park%20Won%20Seok");
		FString fullPath = baseURL2 + "/post" + "?username=" + myName;
		httpReqActor->PostRequest(fullPath);
	/*}*/
}

#pragma region other

//void UProjectPanel::GetImageRequest()
//{
//	if ( httpReqActor != nullptr )
//	{
//		FString url = TEXT("https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcSi9Ccxe-lGF3vbWKaa39Cy7RpKilpEdAmfAg&usqp=CAU");
//		httpReqActor->GetImage(url);
//	}
//}
//
//void UProjectPanel::PostImageRequest()
//{
//	UTexture2D* myTex = Cast<UTexture2D>(img_Http->GetBrush().GetResourceObject());
//
//	if ( myTex != nullptr && httpReqActor != nullptr )
//	{
//		FString fullurl = baseURL2 + "/post/imageShow";
//		httpReqActor->PostImage(fullurl , myTex);
//	}
//}
//
//// 특정 라인의 데이터를 읽는 함수
//void UProjectPanel::ReadCsvSingle()
//{
//	if ( playerInfoTable != nullptr )
//	{
//		FPlayerInfoTable* result = playerInfoTable->FindRow<FPlayerInfoTable>(FName(edit_row->GetText().ToString()) , "Character information");
//
//		if ( result != nullptr )
//		{
//			text_log->SetText(FText::FromString(FString::Printf(TEXT(
//				"Name: %s, Job: %s, HP: %d, MP: %d") , *result->name , *result->job , result->hp , result->mp)));
//		}
//	}
//}
//
//void UProjectPanel::ReadCsvAll()
//{
//	if ( playerInfoTable != nullptr )
//	{
//		TArray<FPlayerInfoTable*> playerInfoList;
//		playerInfoTable->GetAllRows<FPlayerInfoTable>("PlayerDataList" , playerInfoList);
//
//		if ( playerInfoList.Num() > 0 )
//		{
//			FString resultText;
//			for ( FPlayerInfoTable* playerInfo : playerInfoList )
//			{
//				resultText.Append(FString::Printf(TEXT("Name: %s, Job: %s, HP: %d, MP: %d\r\n") , *playerInfo->name , *playerInfo->job , playerInfo->hp , playerInfo->mp));
//			}
//
//			text_log->SetText(FText::FromString(resultText));
//		}
//	}
//}
//
//void UProjectPanel::ReadCsvFile()
//{
//	TArray<FPlayerInfoTable> playerDataList;
//	FString path = FPaths::ProjectContentDir() + "CSVData/PlayerInfoTable.csv";
//
//	UCSVParseLibrary::ReadMyCSV(path , playerDataList);
//
//	UE_LOG(LogTemp , Warning , TEXT("Path: %s") , *path);
//	UE_LOG(LogTemp , Warning , TEXT("List: %d") , playerDataList.Num());
//
//	if ( playerDataList.Num() > 0 )
//	{
//		FString resultText;
//		for ( FPlayerInfoTable playerInfo : playerDataList )
//		{
//			resultText.Append(FString::Printf(TEXT("Name: %s, Job: %s, HP: %d, MP: %d\r\n") , *playerInfo.name , *playerInfo.job , playerInfo.hp , playerInfo.mp));
//		}
//
//		text_log->SetText(FText::FromString(resultText));
//	}
//}

#pragma endregion
