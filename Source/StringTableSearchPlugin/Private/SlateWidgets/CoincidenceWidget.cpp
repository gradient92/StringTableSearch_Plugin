// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/CoincidenceWidget.h"


void SCoincidenceWidget::Construct(const FArguments& InArgs)
{
	if (!InArgs._StringTablesWithCoincidence) return;
	
	//TODO: Rebuild Search Result Widget
		
	TSharedPtr<SVerticalBox> VerticalBox;
	this->ChildSlot
	[
		SNew(SBox)
		[
			SAssignNew(VerticalBox, SVerticalBox)
		]
	];
		
	VerticalBox->AddSlot()
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("DetailsView.CardHeaderTopRounded"))
		[
			SNew(SBox)
			.MinDesiredHeight(20.f)
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
				[
					SNew(STextBlock)
					.Text(FText::FromString(InArgs._StringTablesWithCoincidence->AssetData->AssetName.ToString()))
					.Justification(ETextJustify::Left)
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
			.HeightOverride(30.f)
			[
				SNew(SButton)
				.Text(FText::FromString(Pair.Key + " " + Pair.Value))
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
