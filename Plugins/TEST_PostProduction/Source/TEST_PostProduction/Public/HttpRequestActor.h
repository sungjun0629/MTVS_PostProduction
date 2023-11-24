// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpRequestActor.generated.h"

UCLASS()
class TEST_POSTPRODUCTION_API AHttpRequestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpRequestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SendRequest(const FString url);
	void PostRequest(const FString url);
	//void SaveJson(const FString jsonData);
	//void GetImage(const FString url);
	//void SaveImage(const UTexture2D* tex);
	//void PostImage(const FString url , const UTexture2D* tex);

private:
	void OnReceivedData(FHttpRequestPtr Request , FHttpResponsePtr Response , bool bConnectedSuccessfully);
	void OnPostData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);
	//void OnGetImageData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);
	//void OnPostTextureData(TSharedPtr<IHttpRequest> Request , TSharedPtr<IHttpResponse> Response , bool bConnectedSuccessfully);


};
