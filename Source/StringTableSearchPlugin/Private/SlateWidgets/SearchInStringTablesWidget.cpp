// Copyright Alexander "gradient92" Vashchenko

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
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 0.f, 0.f, 4.f)
		[
			SNew(SBox)
			.HeightOverride(20.f)
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
			.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
			.Padding(FMargin(8.f, 8.f, 4.f, 0.f))
			[
				SAssignNew(ResultsContainer, SModifiedScrollBox)
			]
		]
		/*+SVerticalBox::Slot()
		.AutoHeight()
		.Padding( FMargin( 0.f, 8.f ) )*/
	];
}

void SSearchInStringTablesWidget::OnSearchTextChanged(const FText& Text)
{
	SearchValue = Text.ToString();
}

void SSearchInStringTablesWidget::OnSearchTextCommitted( const FText& Text, ETextCommit::Type CommitType )
{
	if (CommitType != ETextCommit::OnEnter) return;
	
	if (SearchValue.TrimStartAndEnd().IsEmpty()) return;
	
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
			FString CompareString = SourceString.ToLower().Replace(TEXT("\r\n"),TEXT(" "));
			
			if (Key.ToLower().Contains(SearchValue.ToLower()) || CompareString.Contains(SearchValue.ToLower()))
			{
				Coincidences.Add(Key, SourceString);
			}
			return true;
		});
		
		if (Coincidences.Num() > 0)
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