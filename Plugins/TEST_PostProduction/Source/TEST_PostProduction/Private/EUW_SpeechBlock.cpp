// Fill out your copyright notice in the Description page of Project Settings.


#include "EUW_SpeechBlock.h"
#include "Sound/SoundWave.h"
#include "FileHelpers.h"
#include "Interfaces/IAudioFormat.h"

void UEUW_SpeechBlock::CallSetAIVoicePath(const FString& path)
{
	aiVoicePath = path;
	SetAIVoicePath();
}

//USoundWave* UEUW_SpeechBlock::LoadSoundWaveFromFile(const FString& FilePath)
//{
//    // Create an instance of the sound wave
//    USoundWave* SoundWave = NewObject<USoundWave>(USoundWave::StaticClass());
//
//    // Load the raw WAV file data
//    TArray<uint8> RawFileData;
//    if (!FFileHelper::LoadFileToArray(RawFileData , *FilePath) ) {
//        // File loading failed
//        return nullptr;
//    }
//
//    // Create a sound wave from the raw data
//    if ( RawFileData.Num() > 0 ) {
//        FSoundQualityInfo Info;
//        SoundWave->SetWaveData(RawFileData.GetData() , RawFileData.Num());
//        SoundWave->wave
//        SoundWave->Init(Info);
//    }
//    else {
//        // No data loaded
//        return nullptr;
//    }
//
//    return SoundWave;
//}