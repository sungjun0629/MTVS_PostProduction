// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MemoDetailTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMemoDetailTable : public FTableRowBase
{	
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString sequenceTitle;
		UPROPERTY(BlueprintReadWrite , EditAnywhere)
		FString participants;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString content;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString sequencerInfo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString imagePath;

};
