// Copyright Black Ice Mountains. All Rights Reserved.

#include "AssetValidator_NamingValidator.h"

#include "Engine/Blueprint.h"

#define LOCTEXT_NAMESPACE "AssetValidator_NamingValidator"


void UAssetValidator_NamingValidator::PostInitProperties()
{
	Super::PostInitProperties();
	RecompileRegex();
}


void UAssetValidator_NamingValidator::RecompileRegex()
{
	BlueprintClassToPatternIndex.Empty();
	BlueprintClassPatterns.Empty();
	BlueprintClassPatternString.Empty();

	for (const FAssetClassNamingRule &NamingRule : BlueprintClassNamingRules)
	{
		const int32 PatternIndex = BlueprintClassPatterns.Num();
		BlueprintClassToPatternIndex.Add(NamingRule.Class, PatternIndex);
		BlueprintClassPatterns.Add(FRegexPattern(NamingRule.Regex));
		BlueprintClassPatternString.Add(NamingRule.Regex);
	}

	// Default pattern is always last one
	BlueprintClassPatterns.Add(FRegexPattern(DefaultBlueprintPattern));
	BlueprintClassPatternString.Add(DefaultBlueprintPattern);

}


bool UAssetValidator_NamingValidator::CanValidateAsset_Implementation(UObject *InAsset) const
{
	if (!InAsset)
	{
		return false;
	}

	const UPackage *Package = InAsset->GetOutermost();
	if (!Package)
	{
		return false;
	}
	return true;
}


EDataValidationResult UAssetValidator_NamingValidator::ValidateLoadedAsset_Implementation(UObject *InAsset, TArray<FText> &ValidationErrors)
{
	const UPackage *Package = InAsset->GetOutermost();
	const FString PackagePath = Package->GetPathName();
	const FString PackageFileName = FPaths::GetBaseFilename(PackagePath);

	if (auto* Blueprint = Cast<UBlueprint>(InAsset))
	{
		// Default is always last.
		int32 PatternIndex = BlueprintClassPatterns.Num() - 1;
		if (Blueprint->ParentClass)
		{
			if (int32* TypedRegexPatternIndex = BlueprintClassToPatternIndex.Find(Blueprint->ParentClass->GetPathName()))
			{
				PatternIndex = *TypedRegexPatternIndex;
			}
		}

		FRegexMatcher Matcher(BlueprintClassPatterns[PatternIndex], PackageFileName);

		if (!Matcher.FindNext())
		{
			const FString ClassName = Blueprint->ParentClass ? Blueprint->ParentClass->GetPathName() : "None";
			AssetWarning(InAsset, FText::Format(LOCTEXT("BlueprintMustMatchRegex", "Blueprint of class {0} must match {1}"), FText::FromString(ClassName), FText::FromString(BlueprintClassPatternString[PatternIndex])));
		}
	}

	AssetPasses(InAsset);
	return GetValidationResult();
}


#undef LOCTEXT_VALIDATOR
