// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/CoincidenceWidget.h"


void SCoincidenceWidget::Construct(const FArguments& InArgs)
{
	if (!InArgs._StringTablesWithCoincidence) return;
		
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
				.Value(0.2)
				[
					SNew(SBorder)
					.BorderImage(&ElementBorderBrush.Get())
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Margin(FMargin(5.f, 0.f, 0.f, 0.f))
						.Justification(ETextJustify::Left)
						.Text(FText::FromString(Pair.Key))
					]
				]
				+ SSplitter::Slot()
				[
					SNew(SBorder)
					.BorderImage(&ElementBorderBrush.Get())
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Margin(FMargin(5.f, 0.f, 0.f, 0.f))
						.Justification(ETextJustify::Left)
						.Text(FText::FromString(Pair.Value))
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

/*void SCoincidenceWidget::OnSlotResize(float X)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), X);
}*/