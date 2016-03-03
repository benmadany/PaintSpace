// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class PaintSpace : ModuleRules
{

    public PaintSpace(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "LeapMotion", "Leap", "Slate", "SlateCore", "UMG", "ProceduralMeshComponent"/*, "RAWMESH"*/ });

        PublicIncludePaths.AddRange(new string[] { "LeapMotion/Public" });

		//PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"});
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
