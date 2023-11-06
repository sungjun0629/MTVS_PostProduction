// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "WebBrowser/Public/SWebBrowser.h"
#include "WebBrowser/Public/SWebBrowserView.h"
/**
 *
 */
class SSequencePractice : public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SSequencePractice) {}

	SLATE_END_ARGS()

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void GetSequenceAsset();

};
