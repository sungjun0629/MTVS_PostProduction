#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VoiceFileManager.generated.h"


UCLASS(BlueprintType)
class TEST_POSTPRODUCTION_API UVoiceFileManager : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "Save" ))
	static bool SaveArrayText(FString SaveDirectory , FString FileName , TArray<FString> SaveText , bool AllowOverwriting);
	
};
