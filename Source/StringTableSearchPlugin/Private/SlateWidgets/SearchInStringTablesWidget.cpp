// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/SearchInStringTablesWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
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
	StringTableAssets.Empty();
	StringTableCoincidences.Empty();
	AssetRegistryModule->Get().GetAssetsByClass(UStringTable::StaticClass()->GetClassPathName(), StringTableAssets);
	
	for (int i = 0; i < StringTableAssets.Num(); i++)
	{
		FAssetData& AssetData = StringTableAssets[i];
		
		UStringTable* StringTable = Cast<UStringTable>(AssetData.GetAsset());
		if (!StringTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("Не удалось загрузить String Table: %s"), *AssetData.AssetName.ToString());
			continue;
		}
		
		FStringTableConstRef TableData = StringTable->GetStringTable();


		TMap<FString, FString> StringTableMap;
		
		TableData->EnumerateSourceStrings([&StringTableMap](const FString& Key, const FString& SourceString) 
		{
			StringTableMap.Add(Key, SourceString);
			return true;
		});

		TMap<FString, FString> Coincidences;
		for (auto Row : StringTableMap)
		{
			if (Row.Key.ToLower().Contains(SearchValue.ToLower()) || Row.Value.ToLower().Contains(SearchValue.ToLower()))
			{
				Coincidences.Add(Row.Key, Row.Value);
				UE_LOG(LogTemp, Log, TEXT("  Key: %s, Source String: %s"), *Row.Key , *Row.Value);
			}
		}
		if (!Coincidences.IsEmpty())
		{
			StringTableCoincidences.SetNum(StringTableCoincidences.Num() + 1);

			StringTableCoincidences[StringTableCoincidences.Num() - 1].AssetData = &StringTableAssets[i];
			StringTableCoincidences[StringTableCoincidences.Num() - 1].StringMap = Coincidences;
		}
	}
}
