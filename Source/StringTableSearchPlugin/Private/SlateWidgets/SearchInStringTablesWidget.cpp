// Copyright (c) 2025 Alexander "gradient92" Vashchenko
// Licensed under the MIT License. See LICENSE file for details.

#include "SlateWidgets/SearchInStringTablesWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "SlateWidgets/CoincidenceWidget.h"
#include "Widgets/Input/SSearchBox.h"

int32 SModifiedScrollBox::NumSlots() const
{
	return ScrollPanel->Children.Num();
}

SSearchInStringTablesWidget::SSearchInStringTablesWidget()
{
	AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	ColumnFillCoefficients[0] = 0.6f;
	ColumnFillCoefficients[1] = 1.0f;
}

void SSearchInStringTablesWidget::Construct(const FArguments& InArgs)
{
	this->ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
		[
			SNew(SVerticalBox)
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
						SNew(SSearchBox)
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
				.Padding(FMargin(8.f, 8.f, 4.f, 0.f))
				[
					SAssignNew(ResultsContainer, SModifiedScrollBox)
				]
			]

			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding( FMargin( 0.f, 8.f ) )
		]
	];
}

void SSearchInStringTablesWidget::OnSearchTextChanged(const FText& Text)
{
	SearchValue = Text.ToString();
}

void SSearchInStringTablesWidget::OnSearchTextCommitted( const FText& Text, ETextCommit::Type CommitType )
{
	if (CommitType != ETextCommit::OnEnter) return;

	SearchValue = SearchValue.TrimStartAndEnd();
	if (SearchValue.IsEmpty()) return;
	
	ResultsContainer->ClearChildren();
	StringTableAssets.Empty();
	StringTablesWithCoincidences.Empty();
	
	AssetRegistryModule->Get().GetAssetsByClass(*UStringTable::StaticClass()->GetName(), StringTableAssets);
	
	for (FAssetData& AssetData : StringTableAssets)
	{
		UStringTable* StringTable = Cast<UStringTable>(AssetData.GetAsset());
		if (!StringTable) continue;
		
		FStringTableConstRef TableData = StringTable->GetStringTable();
		
		TMap<FString, FString> Coincidences;
		
		TableData->EnumerateSourceStrings([&](const FString& Key, const FString& SourceString) 
		{
			FString CompareString = SourceString.Replace(TEXT("\r\n"),TEXT(" "));

			while (CompareString.Contains(TEXT("  "))) {
				CompareString = CompareString.Replace(TEXT("  "), TEXT(" "));
			}

			while (SearchValue.Contains(TEXT("  "))) {
				SearchValue = SearchValue.Replace(TEXT("  "), TEXT(" "));
			}
            			
			if (Key.Contains(SearchValue, ESearchCase::IgnoreCase) ||
				CompareString.Contains(SearchValue, ESearchCase::IgnoreCase))
			{
				Coincidences.Add(Key, SourceString);
			}
			return true;
		});
		
		if (!Coincidences.IsEmpty())
		{
			StringTablesWithCoincidences.SetNum(StringTablesWithCoincidences.Num() + 1);

			StringTablesWithCoincidences[StringTablesWithCoincidences.Num() - 1].AssetData = &AssetData;
			StringTablesWithCoincidences[StringTablesWithCoincidences.Num() - 1].StringMap = Coincidences;
		}
	}

	for (FStringTable_Coincidences StringTableCoincidence : StringTablesWithCoincidences)
	{
		ResultsContainer->AddSlot()
		[
			SNew(SCoincidenceWidget)
			.StringTablesWithCoincidence(&StringTableCoincidence)
			.ColumnFillCoefficients(ColumnFillCoefficients)
		];
	}

	if (ResultsContainer->NumSlots() == 0)
	{
		ResultsContainer->AddSlot()
		[
			SNew(STextBlock)
			.Text(FText::FromString("No Results found"))
			.Margin(FMargin(18.f, 3.f, 0.f, 0.f))
		];
	}
}