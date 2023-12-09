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

FString UJsonParseLibrary_Plugin::JsonParseToGetURL(const FString& originData)
{
	FString parsedData;
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	// 해독한다.
	if ( FJsonSerializer::Deserialize(reader , result) )
	{
		parsedData = result->GetStringField("uploadUrl");

		if ( parsedData.IsEmpty() )
		{
			parsedData = result->GetStringField("scriptUrl");
		}
	}
	else
	{
		UE_LOG(LogTemp , Warning , TEXT("Failed to Parsing"));
	}

	return parsedData;
}

FSceneCardInfo UJsonParseLibrary_Plugin::JsonParseToGetSceneInfo(const FString& originData)
{
	FSceneCardInfo	parsedData;
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	if ( FJsonSerializer::Deserialize(reader , result) )
	{
		parsedData.story = result->GetStringField("story");
		parsedData.levelLocation = result->GetStringField("levelPosition");
		parsedData.imageBase64 = result->GetStringField("thumbnail");

		UE_LOG(LogTemp,Warning,TEXT("%s"), *parsedData.story)
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed to Parsing"))
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
		const TArray<TSharedPtr<FJsonValue>>* JsonArray;
		if ( result->TryGetArrayField(TEXT("readProjectsResponses") , JsonArray) )
		{
			for ( const TSharedPtr<FJsonValue>& JsonValue : *JsonArray )
			{
				const TSharedPtr<FJsonObject>& ProjectObject = JsonValue->AsObject();
				if ( ProjectObject.IsValid() )
				{
					FProjectUnit Project;
					Project.projectId = ProjectObject->GetIntegerField(TEXT("projectId"));
					Project.projectName = ProjectObject->GetStringField(TEXT("projectName"));
					Project.projectBase64 = ProjectObject->GetStringField(TEXT("posterUrl"));

					// 이미지의 경우 UTexture2D로 변환해주어야 한다. 
					// url을 통해 get을 요청하고
					// -> Utexture2D로 변환을 해준다.  


					parsedProjectData.Add(Project);
				}
			}
		}
	}

	return parsedProjectData;
}


FProjectInfo UJsonParseLibrary_Plugin::JsonPraticularProjectParse(const FString& originData)
{
	FProjectInfo parsedProjectInfo;

	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());;

	if ( FJsonSerializer::Deserialize(reader , result) )
	{
		parsedProjectInfo.projectName = (result->GetStringField("projectName"));
		parsedProjectInfo.description = (result->GetStringField("description"));
		parsedProjectInfo.poster = ( result->GetStringField("poster") );

		const TArray<TSharedPtr<FJsonValue>>* AvatarInfoArray;
		if(result->TryGetArrayField("avatarInfo" , AvatarInfoArray))
		{
			for ( const auto& AvatarInfoValue : *AvatarInfoArray ) {
				
				FAvartarInfo parsedAvartarInfo;
				if ( AvatarInfoValue.IsValid() && AvatarInfoValue->Type == EJson::Object ) {
					const TSharedPtr<FJsonObject>* AvatarInfoObject = &AvatarInfoValue->AsObject();

					int32 AvatarId;
					FString AvatarName , AvatarImage;


					(*AvatarInfoObject )->TryGetNumberField("avatarId" , AvatarId);
					( *AvatarInfoObject )->TryGetStringField("avatarName" , AvatarName);
					( *AvatarInfoObject )->TryGetStringField("avatarImage" , AvatarImage);

					parsedAvartarInfo.avartarID = AvatarId;
					parsedAvartarInfo.avartarName = AvatarName;
					parsedAvartarInfo.avartarBase64 = AvatarImage;

					parsedProjectInfo.avartarInfo.Add(parsedAvartarInfo);
				}
			}
		}

		const TArray<TSharedPtr<FJsonValue>>* StaffInfoArray;
		if ( result->TryGetArrayField("staffInfo" , StaffInfoArray) )
		{
			for ( const auto& AvatarStaffValue : *StaffInfoArray ) {
				
				FStaffInfo parsedStaffInfo;
				if ( AvatarStaffValue.IsValid() && AvatarStaffValue->Type == EJson::Object ) {
					const TSharedPtr<FJsonObject>* AvatarInfoObject = &AvatarStaffValue->AsObject();

					FString staffName;
					FString staffImage , staffRole;


					( *AvatarInfoObject )->TryGetStringField("staffName" , staffName);
					( *AvatarInfoObject )->TryGetStringField("staffImage" , staffImage);
					( *AvatarInfoObject )->TryGetStringField("staffRole" , staffRole);

					parsedStaffInfo.staffName = staffName;
					parsedStaffInfo.staffBase64 = staffImage;
					parsedStaffInfo.staffRole = staffRole;

					parsedProjectInfo.staffInfo.Add(parsedStaffInfo);
				}
			}
		}
		//parsedProjectInfo.staffInfo = (result->GetArrayField("staffInfo"));
	}


	return parsedProjectInfo;
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