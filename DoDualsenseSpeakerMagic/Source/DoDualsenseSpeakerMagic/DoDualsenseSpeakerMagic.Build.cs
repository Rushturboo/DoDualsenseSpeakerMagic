// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class DoDualsenseSpeakerMagic : ModuleRules
{
	public DoDualsenseSpeakerMagic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"DoDualsenseSpeakerMagic/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"AudioMixer"
				// ... add other public dependencies that you statically link with here ...
			}
			);
		// Windows WASAPI dependencies
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDefinitions.Add("WITH_WASAPI=1");
			PublicSystemLibraries.Add("Ole32.lib");
			PublicSystemLibraries.Add("Avrt.lib");
			PublicSystemLibraries.Add("Propsys.lib");
		}
		else
		{
			PublicDefinitions.Add("WITH_WASAPI=0");
		}
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
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
