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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 projectId = 9999;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString projectName = TEXT("untitled");
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString projectUrl = TEXT("untitled");

};

USTRUCT(BlueprintType)
struct FAvartarInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 avartarID = 9999;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString avartarName = TEXT("untitled");
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString avartarImage = TEXT("untitled");

};

USTRUCT(BlueprintType)
struct FStaffInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString staffName = "NoName";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString staffImage = TEXT("untitled");
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString staffRole = TEXT("untitled");
};


USTRUCT(BlueprintType)
struct FProjectInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString projectName = "untitled";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString description = TEXT("untitled");
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString poster = TEXT("untitled");
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FAvartarInfo> avartarInfo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FStaffInfo> staffInfo;

};


UCLASS()
class UJsonParseLibrary_Plugin : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:
	static FString MakeJson(const TMap<FString, FString> source);

	static FString JsonParse(const FString& originData);

	static TArray<FProjectUnit> JsonProjectParse(const FString& originData);

	static FProjectInfo JsonPraticularProjectParse(const FString& originData);

	static TArray<FString> JsonParse3DImage(const FString& originData);
};
