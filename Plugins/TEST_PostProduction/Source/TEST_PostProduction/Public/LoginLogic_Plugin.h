// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

/**
 * 
 */
class TEST_POSTPRODUCTION_API LoginLogic_Plugin
{
public:
	LoginLogic_Plugin();
	~LoginLogic_Plugin();

	 void GetHeaderToken(FString URL);

	void OnGettingToken(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully);
};
