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

public:
	/** Unique ID used to identify this row */
	FName RowId;

	/** Display name of this row */
	FText DisplayName;

	/** The calculated height of this row taking into account the cell data for each column */
	float DesiredRowHeight;

	/** Insertion number of the row */
	uint32 RowNum;

	/** Array corresponding to each cell in this row */
	TArray<FText> CellData;
};


struct FMemoDataTableColumn
{
	/** Unique ID used to identify this column */
	FName ColumnId = "hi";

	/** Display name of this column */
	FText DisplayName = FText::FromString("Bye");

	/** The calculated width of this column taking into account the cell data for each row */
	float DesiredColumnWidth;

	/** The FProperty for the variable in this column */
	const FProperty* Property;
};
