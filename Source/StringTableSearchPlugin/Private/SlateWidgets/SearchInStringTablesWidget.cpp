// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/SearchInStringTablesWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "SlateWidgets/CoincidenceWidget.h"
#include "Widgets/Input/SSearchBox.h"

SSearchInStringTablesWidget::SSearchInStringTablesWidget()
{
	AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
}

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
				.Padding(FMargin(8.f, 8.f, 4.f, 0.f))
				[
					SAssignNew(ResultsContainer, SScrollBox)
				]
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
}

void SSearchInStringTablesWidget::OnSearchTextCommitted( const FText& Text, ETextCommit::Type CommitType )
{
	ResultsContainer->ClearChildren();
	StringTableAssets.Empty();
	StringTablesWithCoincidences.Empty();
	AssetRegistryModule->Get().GetAssetsByClass(UStringTable::StaticClass()->GetClassPathName(), StringTableAssets);
	
	for (FAssetData& AssetData : StringTableAssets)
	{
		UStringTable* StringTable = Cast<UStringTable>(AssetData.GetAsset());
		if (!StringTable) continue;
		
		FStringTableConstRef TableData = StringTable->GetStringTable();
		
		TMap<FString, FString> Coincidences;
		
		TableData->EnumerateSourceStrings([&](const FString& Key, const FString& SourceString) 
		{
			if (Key.ToLower().Contains(SearchValue.ToLower()) || SourceString.ToLower().Contains(SearchValue.ToLower()))
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
		];
	}
	
}