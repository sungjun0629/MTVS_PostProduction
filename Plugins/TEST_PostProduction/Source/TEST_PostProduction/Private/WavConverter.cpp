// Fill out your copyright notice in the Description page of Project Settings.

#include "Runtime/Engine/Classes/Sound/SoundWave.h"
#include "Misc/FileHelper.h"
#include "WavConverter.h"

void UWavConverter::CreateWavFromSoundWave(USoundWave* SoundWave, FString AddedPath)
{
    if ( SoundWave )
    {
        TArray<uint8> WavData;

        // Assuming SoundWave contains audio data
        // Retrieve the raw PCM audio data from the SoundWave
        const uint8* RawWaveData = SoundWave->GetResourceData();
        const int32 RawDataSize = SoundWave->GetResourceSize();

        // Convert the sound wave data to the .wav format
        // ... (conversion process)

        // Save the .wav file
        const FString FilePath = FPaths::ProjectContentDir() + AddedPath + ".wav";
        FFileHelper::SaveArrayToFile(WavData , *FilePath);
    }
}