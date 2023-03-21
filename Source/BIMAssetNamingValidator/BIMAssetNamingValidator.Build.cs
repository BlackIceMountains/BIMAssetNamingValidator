// Copyright Black Ice Mountains. All Rights Reserved.

using UnrealBuildTool;

public class BIMAssetNamingValidator: ModuleRules
{
    public BIMAssetNamingValidator(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "DataValidation",
            "Engine",
        });
    }
}
