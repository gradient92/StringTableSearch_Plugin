// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FStringTable_Coincidences
{
	FAssetData* AssetData;
	TMap<FString, FString> StringMap;
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
	TWeakPtr<SVerticalBox> MainVerticalBox;
	TSharedPtr<SSearchBox> SearchTextField;

	FString	SearchValue;

	TArray<FAssetData> StringTableAssets;

	FAssetRegistryModule* AssetRegistryModule;
	
	TArray<FStringTable_Coincidences> StringTableCoincidences;

	void OnSearchTextChanged(const FText& Text);

	void OnSearchTextCommitted( const FText& Text, ETextCommit::Type CommitType );
};
