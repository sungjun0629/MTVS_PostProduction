#include "UW_SingleScript.h"
#include "Components/TextBlock.h"

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
