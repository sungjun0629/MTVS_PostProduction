// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSlider.h"

/**
 * 
 */

class SSoundConvertWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_END_ARGS()

public:

	float CurrentPlaybackPosition = 0.0f;

	TSharedPtr<SSlider> originalSlider;

	TSharedPtr<SSlider> convertedSlider;

	FString SeletedVoicePath;

	USoundWave* SoundWave;

	bool bIsPlaying = false;

	bool bIsOriginalChecked;

	bool bIsConvertedChecked;

	void Construct(const FArguments& InArgs);

	FReply OnLoadClicked();

	FReply OnConvertClicked();

	FReply OnPlayClicked();

	FReply OnStopClicked();

	void OnCheckOriginalStateChanged(ECheckBoxState NewState);

	void OnCheckConvertedStateChanged(ECheckBoxState NewState);

	void OnSliderChanged(float playRate);

	void UpdateSlider(float deltaTime);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	FVector GetMousePositionInEditor();

};