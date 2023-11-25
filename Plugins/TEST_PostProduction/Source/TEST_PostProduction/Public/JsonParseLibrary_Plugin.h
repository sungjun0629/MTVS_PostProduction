// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLibrary_Plugin.generated.h"

USTRUCT(BlueprintType)
struct FProjectUnit
{
	GENERATED_USTRUCT_BODY()

public:
	int32 projectId = 9999;
	FString projectName = TEXT("untitled");
	FString projectUrl = TEXT("untitled");

};


UCLASS()
class UJsonParseLibrary_Plugin : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:
	static FString MakeJson(const TMap<FString, FString> source);

	static FString JsonParse(const FString& originData);

	static TArray<FProjectUnit> JsonProjectParse(const FString& originData);

	static TArray<FString> JsonParse3DImage(const FString& originData);
};
