// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TEST_PostProduction : ModuleRules
{
	public TEST_PostProduction(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
            System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "/Source/Editor/Blutility/Private",
            System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "/Source/Runtime/CEF3Utils/Private",
            System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "/Source/Editor/DataTableEditor/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Blutility",
                "WebBrowser",
                "EditorWidgets",
                "Json",
				"JsonUtilities",
				"HTTP",
                "DesktopPlatform",
                "ToolWidgets",
                "LevelSequence",
                "DataTableEditor",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				"UMGEditor",
                "DataTableEditor",
                "ApplicationCore",
                "PropertyEditor",
				"AssetTools",
                "LiveLinkMultiUser"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
