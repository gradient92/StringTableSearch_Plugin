// Copyright (c) 2025 Alexander "gradient92" Vashchenko
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "SearchInStringTablesWidget.h"

/**
 * 
 */
class STRINGTABLESEARCHPLUGIN_API SCoincidenceWidget: public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SCoincidenceWidget) {}

		SLATE_ARGUMENT(float*, ColumnFillCoefficients)
		SLATE_ARGUMENT(FStringTable_Coincidences*, StringTablesWithCoincidence)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);

private:
	FAssetData* AssetData = nullptr;
	
	TSharedPtr<SButton> ExpanderArrow;
	bool isExpanded = true;

	TArray<TSharedPtr<SBox>> Boxes;

	float* ColumnFillCoefficients = nullptr;
	
	TSharedRef<FSlateBrush> MainBorderBrush = MakeShared<FSlateBrush>();
	TSharedRef<FSlateBrush> ElementBorderBrush = MakeShared<FSlateBrush>();
	
	FReply OnExpanderClicked();
	
	const FSlateBrush* GetExpanderImage() const;
	
	void OnSlotResize(float FillCoefficient, int32 ColumnIndex) const;

	float GetColumnFillCoefficient(int32 ColumnIndex) const;
	
	FReply OnStringTableMouseDoubleClick(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const;

	FReply OnStringTableMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

	FReply OnElementMouseDoubleClick(const FGeometry& Geometry, const FPointerEvent& MouseEvent, FString Key) const;
	
	FReply OnElementMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent, TPair<FString, FString> Pair);
};
