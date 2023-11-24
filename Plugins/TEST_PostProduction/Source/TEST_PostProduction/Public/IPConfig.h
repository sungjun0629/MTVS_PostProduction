// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SequencerPractice.h"
#include "MemoTableEditor.h"
#include "SMemoTableListViewRow.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSequencerNameChanged , FString)

class FMemoTableEditor;
/**
 * 
 */
class TEST_POSTPRODUCTION_API IPConfig
{
public:
	IPConfig();
	~IPConfig();

	inline static FString StaticVariable = "http://postproduction.life";

	inline static FString Token;

	inline static FString SequenceName;

	inline static FString ImagePath;

	inline static TSharedPtr<FMemoTableEditor> MemoTableEditor = MakeShared<FMemoTableEditor>();

	inline static TSharedPtr<SMemoTableListViewRow> ListView = MakeShared<SMemoTableListViewRow>();

	inline static void changeSequenceName(FString sequenceName);

	inline static FOnSequencerNameChanged sequnencerNameChanged;

	inline static FString MemoContentUUID;
};
