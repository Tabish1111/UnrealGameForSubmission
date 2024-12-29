// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class FPS_BoxShooter : ModuleRules
{
    public FPS_BoxShooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });

        // FBX SDK configuration
        string FBXSDKPath = @"C:\Program Files\Autodesk\FBX\FBX SDK\2020.3.2";
        if (Directory.Exists(FBXSDKPath))
        {
            string FBXIncludePath = Path.Combine(FBXSDKPath, "include");
            string FBXLibPath = Path.Combine(FBXSDKPath, "lib", "vs2022", "x64", "release");

            if (Directory.Exists(FBXIncludePath) && Directory.Exists(FBXLibPath))
            {
                PublicIncludePaths.Add(FBXIncludePath);
                PublicSystemLibraryPaths.Add(FBXLibPath);
                PublicAdditionalLibraries.Add("libfbxsdk-md.lib");
                PublicDefinitions.Add("WITH_FBX_SDK=1");
            }
            else
            {
                System.Console.WriteLine("Warning: FBX SDK include or lib directory not found:");
                System.Console.WriteLine($"Include Path: {FBXIncludePath}");
                System.Console.WriteLine($"Library Path: {FBXLibPath}");
            }
        }
        else
        {
            System.Console.WriteLine("Warning: FBX SDK path not found: " + FBXSDKPath);
        }

        // Engine module configuration
        bTreatAsEngineModule = false;
    }
}