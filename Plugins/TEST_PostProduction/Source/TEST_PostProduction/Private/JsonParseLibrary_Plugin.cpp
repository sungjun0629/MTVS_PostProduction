// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLibrary_Plugin.h"
#include "Json.h"

FString UJsonParseLibrary_Plugin::MakeJson(const TMap<FString, FString> source)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> KeyValue : source)
	{
		JsonObject->SetStringField(KeyValue.Key, KeyValue.Value);
	}

	FString JsonData;

	TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&JsonData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return JsonData;
}

FString UJsonParseLibrary_Plugin::JsonParse(const FString& originData)
{
	FString parsedData;

	// Reader를 만든다.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);

	// reader로 파싱된 결과를 담을 json Object를 선언한다.
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	// 해독한다.
	if (FJsonSerializer::Deserialize(reader, result))
	{
		parsedData = result->GetStringField("voiceUrl");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Parsing"));
	}

	return parsedData;
}
