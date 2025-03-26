// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SearchInStringTablesWidget.h"

/**
 * 
 */
class STRINGTABLESEARCHPLUGIN_API SCoincidenceWidget: public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SCoincidenceWidget) {}
		
		SLATE_ARGUMENT(FStringTable_Coincidences*, StringTablesWithCoincidence)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);
	
	FReply OnExpanderClicked();

	bool isExpanded = true;
	
	TSharedPtr<SButton> ExpanderArrow;

	TArray<TSharedPtr<SBox>> Boxes;
	
	const FSlateBrush* GetExpanderImage() const;
};
