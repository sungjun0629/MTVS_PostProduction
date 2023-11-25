// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLibrary_Plugin.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Dom/JsonObject.h"

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

TArray<FProjectUnit> UJsonParseLibrary_Plugin::JsonProjectParse(const FString& originData)
{
	TArray<FProjectUnit> parsedProjectData;


	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());;

	if ( FJsonSerializer::Deserialize(reader , result) )
	{
		// Json 해독 
		UE_LOG(LogTemp,Warning,TEXT("Deserialize"));
		const TArray<TSharedPtr<FJsonValue>>* JsonArray;
		if ( result->TryGetArrayField(TEXT("readProjectsResponses") , JsonArray) )
		{
			UE_LOG(LogTemp,Warning,TEXT("get Array"));
			for ( const TSharedPtr<FJsonValue>& JsonValue : *JsonArray )
			{
				const TSharedPtr<FJsonObject>& ProjectObject = JsonValue->AsObject();
				UE_LOG(LogTemp,Warning,TEXT("hi"));
				if ( ProjectObject.IsValid() )
				{
					FProjectUnit Project;
					Project.projectId = ProjectObject->GetIntegerField(TEXT("projectId"));
					Project.projectName = ProjectObject->GetStringField(TEXT("projectName"));
					Project.projectUrl = ProjectObject->GetStringField(TEXT("posterUrl"));

					parsedProjectData.Add(Project);
				}
			}
		}
	}

	return parsedProjectData;
}


TArray<FString> UJsonParseLibrary_Plugin::JsonParse3DImage(const FString& originData)
{
	TArray<FString> parsedData;


	// Reader를 만든다.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);

	// reader로 파싱된 결과를 담을 json Object를 선언한다.
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	// 해독한다.
	if ( FJsonSerializer::Deserialize(reader , result) )
	{
		parsedData.Add(result->GetStringField("albedoUrl"));
		parsedData.Add(result->GetStringField("meshMtl"));
		parsedData.Add(result->GetStringField("meshObj"));
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("Failed to Parsing 3D Image"));
	}

	return parsedData;
}