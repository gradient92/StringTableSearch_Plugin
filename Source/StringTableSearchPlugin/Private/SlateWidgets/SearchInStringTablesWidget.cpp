// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/SearchInStringTablesWidget.h"

#include "Widgets/Input/SSearchBox.h"

void SSearchInStringTablesWidget::Construct(const FArguments& InArgs)
{
	this->ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
		[
			SAssignNew(MainVerticalBox, SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.f, 5.f, 8.f, 5.f)
			[
				SNew(SBox)
				.HeightOverride(26.f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.FillWidth(1)
					[
						SAssignNew(SearchTextField, SSearchBox)
						.HintText(FText::FromString(TEXT("Enter Key or Source String to find references...")))
						.OnTextChanged(this, &SSearchInStringTablesWidget::OnSearchTextChanged)
						.OnTextCommitted(this, &SSearchInStringTablesWidget::OnSearchTextCommitted)
						.DelayChangeNotificationsWhileTyping(false)
					]
				]
			]
			+SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("Brushes.Recessed"))
			]

			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding( FMargin( 16.f, 8.f ) )
			[
				SNew(SHorizontalBox)
				
				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(FMargin(12.f, 8.f, 16.f, 8.f))
				.VAlign(VAlign_Center)
			]
		]
	];
}

void SSearchInStringTablesWidget::OnSearchTextChanged(const FText& Text)
{
	SearchValue = Text.ToString();

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10.f,
			FColor::Yellow,
			FString::Printf(TEXT("typing"))
			);
	}
}

void SSearchInStringTablesWidget::OnSearchTextCommitted( const FText& Text, ETextCommit::Type CommitType )
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10.f,
			FColor::Orange,
			FString::Printf(TEXT("ENTER"))
			);
	}
}
