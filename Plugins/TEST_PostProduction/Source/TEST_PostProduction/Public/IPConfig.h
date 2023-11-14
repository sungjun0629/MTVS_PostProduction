// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TEST_POSTPRODUCTION_API IPConfig
{
public:
	IPConfig();
	~IPConfig();

	inline static FString StaticVariable = "http://192.168.0.35:4493";

	inline static FString Token;

	inline static FString SequenceName;

	inline static FString ImagePath;
};
