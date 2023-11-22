// Fill out your copyright notice in the Description page of Project Settings.


#include "IPConfig.h"

IPConfig::IPConfig()
{
}

IPConfig::~IPConfig()
{
}


void IPConfig::changeSequenceName(FString sequenceName)
{
	SequenceName = sequenceName;
	sequnencerNameChanged.Broadcast(sequenceName);
}
