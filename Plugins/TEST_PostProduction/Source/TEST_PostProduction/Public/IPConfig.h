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

	inline static FString StaticVariable = "http://121.165.108.236:4492";

	inline static FString Token;
};
