#include "VoiceFileManager.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

bool UVoiceFileManager::SaveArrayText(FString SaveDirectory , FString FileName , TArray<FString> SaveText , bool AllowOverwriting = false)
{
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if ( !AllowOverwriting )
	{
		if ( FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory ))
		{
			return false;
		}
	}

	FString FinalString = "";
	for ( FString& Each : SaveText )
	{
		FinalString += Each;
		FinalString += LINE_TERMINATOR;
	}

	return FFileHelper::SaveStringToFile(FinalString , *SaveDirectory);
}
