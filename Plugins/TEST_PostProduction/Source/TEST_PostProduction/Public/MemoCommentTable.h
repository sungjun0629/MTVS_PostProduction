// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MemoCommentTable.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FMemoCommentTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString ContentUUID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString comment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString author;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString createdAt;
};
