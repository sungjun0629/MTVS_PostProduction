// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLibrary_Plugin.generated.h"

UCLASS()
class UJsonParseLibrary_Plugin : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:
	static FString MakeJson(const TMap<FString, FString> source);
};
