// Copyright (c) 2025 Alexander "gradient92" Vashchenko
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SScrollBox.h"

struct FStringTable_Coincidences
{
	FAssetData* AssetData;
	TArray<TPair<FString, FString>> StringMap;
};

class SModifiedScrollBox : public SScrollBox
{
public:
	int32 NumSlots() const;
};

class FAssetRegistryModule;
/**
 * 
 */
class STRINGTABLESEARCHPLUGIN_API SSearchInStringTablesWidget: public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SSearchInStringTablesWidget) {}
		
		SLATE_ARGUMENT(TSharedPtr<SDockTab>, ContainingTab)
	SLATE_END_ARGS()

public:
	SSearchInStringTablesWidget();
	
	void Construct(const FArguments& InArgs);

private:
	FString	SearchValue;

	TArray<FAssetData> StringTableAssets;

	FAssetRegistryModule* AssetRegistryModule;
	
	TArray<FStringTable_Coincidences> StringTablesWithCoincidences;

	TSharedPtr<SModifiedScrollBox> ResultsContainer;

	float ColumnFillCoefficients[2];

	void OnSearchTextChanged(const FText& Text);

	void OnSearchTextCommitted( const FText& Text, ETextCommit::Type CommitType );
};
