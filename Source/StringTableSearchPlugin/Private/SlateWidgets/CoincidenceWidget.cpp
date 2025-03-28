// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/CoincidenceWidget.h"

#include "AssetToolsModule.h"

void SCoincidenceWidget::Construct(const FArguments& InArgs)
{
	if (!InArgs._StringTablesWithCoincidence) return;
	AssetData = InArgs._StringTablesWithCoincidence->AssetData;
	
	if (!InArgs._ColumnFillCoefficients) return;
	ColumnFillCoefficients = InArgs._ColumnFillCoefficients;
	
	TSharedPtr<SVerticalBox> VerticalBox;
	this->ChildSlot
	[
		SNew(SBox)
		[
			SAssignNew(VerticalBox, SVerticalBox)
		]
	];

	MainBorderBrush->TintColor = FSlateColor(FLinearColor(FColor::FromHex(TEXT("2F2F2FFF"))));
	ElementBorderBrush->TintColor = FSlateColor(FLinearColor(FColor::FromHex(TEXT("242424FF"))));
	
	VerticalBox->AddSlot()
	[
		SNew(SBox)
		.Padding(0.f, 0.f, 0.f, 1.f)
		.HeightOverride(28.f)
		[
			SNew(SBorder)
			.BorderImage(&MainBorderBrush.Get())
			.VAlign(VAlign_Center)
			.OnMouseDoubleClick(this, &SCoincidenceWidget::OnMouseClick)
			[
				SNew(SBox)
				.VAlign(VAlign_Center)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.f, 0.f, 0.f, 0.f)
					[
						SAssignNew(ExpanderArrow, SButton)
						.ButtonStyle(FCoreStyle::Get(), "NoBorder")
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.ClickMethod(EButtonClickMethod::MouseDown)
						.OnClicked(this, &SCoincidenceWidget::OnExpanderClicked)
						.IsFocusable(false)
						[
							SNew(SImage)
							.Image( this, &SCoincidenceWidget::GetExpanderImage)
							.ColorAndOpacity(FSlateColor::UseSubduedForeground())
						]
					]
            				
					+ SHorizontalBox::Slot()
					.Padding(5.f, 0.f, 0.f, 0.f)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(InArgs._StringTablesWithCoincidence->AssetData->AssetName.ToString()))
						.TextStyle(FAppStyle::Get(), "DetailsView.CategoryTextStyle")
						.Justification(ETextJustify::Left)
					]
				]
			]
		]
		
	];
		
	for (auto Pair : InArgs._StringTablesWithCoincidence->StringMap)
	{
		TSharedPtr<SBox> Box;

		/* Trims spaces at the beginning */
		//Pair.Key = Pair.Key.TrimStart();
		//Pair.Value = Pair.Value.TrimStart();
		
		Pair.Key.ReplaceInline(TEXT("\r\n"), TEXT("[↓↓↓]"));
		Pair.Value.ReplaceInline(TEXT("\r\n"), TEXT("[↓↓↓]"));
		
		TAttribute<float> FillCoefficient_0, FillCoefficient_1;
		{
			FillCoefficient_0.Bind(TAttribute<float>::FGetter::CreateSP(this, &SCoincidenceWidget::GetColumnFillCoefficient, 0));
			FillCoefficient_1.Bind(TAttribute<float>::FGetter::CreateSP(this, &SCoincidenceWidget::GetColumnFillCoefficient, 1));
		}
		VerticalBox->AddSlot()
		[
			SAssignNew(Box, SBox)
			.Visibility(isExpanded ? EVisibility::Visible : EVisibility::Collapsed)
			.HeightOverride(28.f)
			.Padding(2.f, 0.f, 2.f, 1.f)
			[
				SNew(SSplitter)
				.Style(FAppStyle::Get(), "DetailsView.Splitter")
				.PhysicalSplitterHandleSize(1.0f)
				.HitDetectionSplitterHandleSize(10.0f)
				.Clipping(EWidgetClipping::ClipToBoundsAlways)
				+ SSplitter::Slot()
				.Value(FillCoefficient_0)
				.OnSlotResized(SSplitter::FOnSlotResized::CreateSP(this, &SCoincidenceWidget::OnSlotResize, 0))
				[
					SNew(SBorder)
					.BorderImage(&ElementBorderBrush.Get())
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Margin(FMargin(5.f, 0.f, 0.f, 0.f))
						.Justification(ETextJustify::Left)
						.Text(FText::FromString(Pair.Key))
						.AutoWrapText(false)
						.WrapTextAt(0)
						.Clipping(EWidgetClipping::ClipToBounds)
						.OverflowPolicy(ETextOverflowPolicy::Ellipsis)
					]
				]
				+ SSplitter::Slot()
				.Value(FillCoefficient_1)
				.OnSlotResized(SSplitter::FOnSlotResized::CreateSP(this, &SCoincidenceWidget::OnSlotResize, 1))
				[
					SNew(SBorder)
					.BorderImage(&ElementBorderBrush.Get())
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Margin(FMargin(5.f, 0.f, 0.f, 0.f))
						.Justification(ETextJustify::Left)
						.Text(FText::FromString(Pair.Value))
						.AutoWrapText(false)
						.WrapTextAt(0)
						.Clipping(EWidgetClipping::ClipToBounds)
						.OverflowPolicy(ETextOverflowPolicy::Ellipsis)
					]
				]
				
			]
		];
		
		Boxes.Add(Box);
	}
}

FReply SCoincidenceWidget::OnExpanderClicked()
{
	if (isExpanded) isExpanded = false;
	else isExpanded = true;

	for (auto Box : Boxes)
	{
		Box->SetVisibility(isExpanded ? EVisibility::Visible : EVisibility::Collapsed);
	}
	
	return FReply::Handled();
}

const FSlateBrush* SCoincidenceWidget::GetExpanderImage() const
{
	FName ResourceName;
	if (isExpanded)
	{
		if (ExpanderArrow->IsHovered())
		{
			static const FName ExpandedHoveredName = "TreeArrow_Expanded_Hovered";
			ResourceName = ExpandedHoveredName;
		}
		else
		{
			static const FName ExpandedName = "TreeArrow_Expanded";
			ResourceName = ExpandedName;
		}
	}
	else
	{
		if (ExpanderArrow->IsHovered())
		{
			static const FName CollapsedHoveredName = "TreeArrow_Collapsed_Hovered";
			ResourceName = CollapsedHoveredName;
		}
		else
		{
			static const FName CollapsedName = "TreeArrow_Collapsed";
			ResourceName = CollapsedName;
		}
	}

	return FAppStyle::Get().GetBrush(ResourceName);
}

void SCoincidenceWidget::OnSlotResize(float FillCoefficient, int32 ColumnIndex) const
{
	ColumnFillCoefficients[ColumnIndex] = FillCoefficient;
}

float SCoincidenceWidget::GetColumnFillCoefficient(int32 ColumnIndex) const
{
	ensure(ColumnIndex == 0 || ColumnIndex == 1);
 	return ColumnFillCoefficients[ColumnIndex];
}

FReply SCoincidenceWidget::OnMouseClick(const FGeometry& Geometry, const FPointerEvent& PointerEvent) const
{
	TArray<UObject*> AssetsToOpenArray;
	AssetsToOpenArray.Add(AssetData->GetAsset());  
	
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get()
	.OpenEditorForAssets(AssetsToOpenArray);
	
	return FReply::Unhandled();
}
