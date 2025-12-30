// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Last_Drop : ModuleRules
{
	public Last_Drop(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
                new string[] 
                {
                    "Core", "InputCore", "EnhancedInput","CoreUObject","AdvancedSessions", "AdvancedSteamSessions"
                }
            );

		PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "Engine",
                    "RHI","RenderCore","ApplicationCore","Slate", "SlateCore",

                }
            );

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
