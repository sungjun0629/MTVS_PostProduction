// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WavConverter.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TEST_POSTPRODUCTION_API UWavConverter : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateWavFromSoundWave(USoundWave* SoundWave, FString AddedPath);
};
