#include "UW_SingleScript.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

void UUW_SingleScript::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUW_SingleScript::GetScriptTable(FString WriteCategory , FString WriteSpeaker , FString WriteScript , FString WriteLocation)
{
	Category = WriteCategory;
	Speaker = WriteSpeaker;
	Script = WriteScript;
	Location = WriteLocation;
}

void UUW_SingleScript::SetTextToUI(UTextBlock* TextBlock, FString Data)
{
	FText NoneText = FText::FromString(TEXT(" "));
	if ( Data == "없음" )
	{
		TextBlock->SetText(NoneText);
	}
	FText DataText = FText::FromString(Data);
	TextBlock->SetText(DataText);
}

void UUW_SingleScript::GetScriptDataFromSystem(FString SystemCategory , FString SystemSpeaker , FString SystemScript , FString SystemLocation , FString SystemSceneNum)
{
	SceneNumber = UKismetStringLibrary::LeftChop(SystemSceneNum , 2);

	SetTextToUI(T_Category , SystemCategory);

	if ( SystemCategory == "장면" )
	{
		IsSceneNumber(SystemSpeaker , SystemLocation);
	}
	else
	{
		IsNotSceneNumber(SystemSceneNum , SystemSpeaker , SystemScript , SystemLocation);
	}
}

void UUW_SingleScript::IsSceneNumber(FString SystemSpeaker , FString SystemLocation)
{
	SetTextToUI(T_SceneNum , SystemSpeaker);
	SetTextToUI(T_Speaker , "");
	SetTextToUI(T_Script , "");
	SetTextToUI(T_Location , SystemLocation);
}

void UUW_SingleScript::IsNotSceneNumber(FString SystemSceneNum , FString SystemSpeaker , FString SystemScript , FString SystemLocation)
{
	SetTextToUI(T_SceneNum , SystemSceneNum);
	SetTextToUI(T_Speaker , SystemSpeaker);
	SetTextToUI(T_Script , SystemScript);
	SetTextToUI(T_Location , SystemLocation);
}
