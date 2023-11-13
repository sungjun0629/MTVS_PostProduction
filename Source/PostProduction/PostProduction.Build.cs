// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PostProduction : ModuleRules
{
	public PostProduction(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HTTP", "Blutility", "UMG", "Slate", "SlateCore", "Json", "JsonUtilities", "TEST_PostProduction" });
	}
}
