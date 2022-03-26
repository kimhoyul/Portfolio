// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Portfolio : ModuleRules
{
	public Portfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
		
		PublicIncludePaths.AddRange(new string[]
		{
			"Portfolio/Public/Player",
			"Portfolio/Public/UI",
			"Portfolio/Public/UI/Inventory",
			"Portfolio/Public/Animations",
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

	}
}
