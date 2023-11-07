// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MemoDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMemoDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString sequenceName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString title;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString content;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString createdAt;
};
