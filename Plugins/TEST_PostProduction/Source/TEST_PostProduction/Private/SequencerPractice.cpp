// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencerPractice.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "LevelSequence/Public/LevelSequence.h"

void SSequencePractice::Construct(const FArguments& InArgs)
{
	GetSequenceAsset();
}

void SSequencePractice::GetSequenceAsset()
{
	// Search Test
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

	FARFilter Filter;
	Filter.ClassNames.Add(ULevelSequence::StaticClass()->GetFName());

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	for (const FAssetData& Asset : AssetList)
	{
		// Use the asset as needed
		ULevelSequence* LevelSequence = Cast<ULevelSequence>(Asset.GetAsset());
		if (LevelSequence)
		{
			// Perform operations on the Level Sequence
			UE_LOG(LogTemp, Warning, TEXT("LevelSequence : %s"), *LevelSequence->GetName());
		}
	}
}
