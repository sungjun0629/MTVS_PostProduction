// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ProjectTable.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct  FProjectTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 projectID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString imagePath;
};
