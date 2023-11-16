// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SequencerPractice.h"

/**
 * 
 */
class TEST_POSTPRODUCTION_API IPConfig
{
public:
	IPConfig();
	~IPConfig();

	inline static FString StaticVariable = "http://121.165.108.236:4492";

	inline static FString Token;

	inline static FString SequenceName;

	inline static FString ImagePath;

	inline static SSequencePractice* sequencerMemo;
};
