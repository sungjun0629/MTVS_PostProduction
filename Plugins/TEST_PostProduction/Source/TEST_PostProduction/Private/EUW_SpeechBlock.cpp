// Fill out your copyright notice in the Description page of Project Settings.


#include "EUW_SpeechBlock.h"

void UEUW_SpeechBlock::CallSetAIVoicePath(const FString& path)
{
	aiVoicePath = path;
	SetAIVoicePath();
}
