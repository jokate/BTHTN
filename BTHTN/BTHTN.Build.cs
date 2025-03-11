// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BTHTN : ModuleRules
{
	public BTHTN(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange( new string[] { "BTHTN"});
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameplayTags", "GameplayAbilities", "AIModule", "GameplayTasks"});
	}
}
