#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLibrary.generated.h"


UCLASS()
class POSTPRODUCTION_API UJsonParseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FString MakeJson(const TMap<FString, FString> source);
};
