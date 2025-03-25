// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class STRINGTABLESEARCHPLUGIN_API SSearchInStringTablesWidget: public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SSearchInStringTablesWidget) {}
		
		SLATE_ARGUMENT(TSharedPtr<SDockTab>, ContainingTab)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TWeakPtr<SVerticalBox> MainVerticalBox;
	TSharedPtr<SSearchBox> SearchTextField;

	FString	SearchValue;

	void OnSearchTextChanged(const FText& Text);

	void OnSearchTextCommitted( const FText& Text, ETextCommit::Type CommitType );
};
