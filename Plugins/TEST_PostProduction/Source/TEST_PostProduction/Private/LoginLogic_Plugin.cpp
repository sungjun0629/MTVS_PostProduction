// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginLogic_Plugin.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"

LoginLogic_Plugin::LoginLogic_Plugin()
{
}

LoginLogic_Plugin::~LoginLogic_Plugin()
{
}

void LoginLogic_Plugin::GetHeaderToken(FString URL)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	FString accessToken;

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindRaw(this, &LoginLogic_Plugin::OnGettingToken);
	Request->ProcessRequest();
}

void LoginLogic_Plugin::OnGettingToken(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid() && NextStepBool == false)
	{
		NextStepBool = true;
		// Parse the response content to extract the token
		TArray<FString> ResponseValues = Response->GetAllHeaders();

		int32 ResponseCode = Response->GetResponseCode();

		UE_LOG(LogTemp, Warning, TEXT("Response Code : %d"), ResponseCode);
		
		if (ResponseCode == 200)
		{
			for (FString value : ResponseValues)
			{
				UE_LOG(LogTemp, Warning, TEXT("Response Value : %s"), *value);
			}
		}
	}
}
