#include "JsonParseLibrary.h"
#include "Json.h"

FString UJsonParseLibrary::MakeJson(const TMap<FString, FString> source)
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
