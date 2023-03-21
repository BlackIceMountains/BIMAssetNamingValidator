// Copyright Black Ice Mountains. All Rights Reserved.

#pragma once

#include "EditorValidatorBase.h"
#include "Internationalization/Regex.h"
#include "AssetValidator_NamingValidator.generated.h"

USTRUCT()
struct FAssetClassNamingRule
{
	GENERATED_BODY()

	UPROPERTY()
	FString Class;
	UPROPERTY()
	FString Regex;
};

UCLASS(Config=BIMAssetNamingValidator, DefaultConfig)
class UAssetValidator_NamingValidator : public UEditorValidatorBase
{
	GENERATED_BODY()

public:

	virtual void PostInitProperties() override;

protected:
	//~UEditorValidatorBase interface
	virtual bool CanValidateAsset_Implementation(UObject *InAsset) const override;
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(UObject *InAsset, TArray<FText> &ValidationErrors) override;
	//~End of UEditorValidatorBase interface

private:

	void RecompileRegex();

	// CONFIG~
	UPROPERTY(config)
	FString DefaultBlueprintPattern;

	UPROPERTY(config)
	TArray<FAssetClassNamingRule> BlueprintClassNamingRules;

	// COMPILED REGEX DATA~

	TMap<FString, int32> BlueprintClassToPatternIndex;
	TArray<FRegexPattern> BlueprintClassPatterns;
	TArray<FString> BlueprintClassPatternString;
};
