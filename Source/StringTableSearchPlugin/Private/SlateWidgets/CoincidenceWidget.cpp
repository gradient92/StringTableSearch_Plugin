// Copyright (c) 2025 Alexander "gradient92" Vashchenko
// Licensed under the MIT License. See LICENSE file for details.

#include "SlateWidgets/CoincidenceWidget.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Internationalization/StringTable.h"
#include "Widgets/Input/SSearchBox.h"

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

	MainBorderBrush->TintColor = FSlateColor(FLinearColor(FColor::FromHex(TEXT("303030FF"))));
	ElementBorderBrush->TintColor = FSlateColor(FLinearColor(FColor::FromHex(TEXT("3E3E3EFF"))));
	
	VerticalBox->AddSlot()
	[
		SNew(SBox)
		.Padding(FMargin(0.f, 0.f, 0.f, 1.f))
		.HeightOverride(28.f)
		[
			SNew(SBorder)
			.BorderImage(&MainBorderBrush.Get())
			.VAlign(VAlign_Center)
			.OnMouseDoubleClick(this, &SCoincidenceWidget::OnStringTableMouseDoubleClick)
			.OnMouseButtonUp(this, &SCoincidenceWidget::OnStringTableMouseButtonUp)
			[
				SNew(SBox)
				.VAlign(VAlign_Center)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(9.f, 3.f, 0.f, 0.f)
					[
						SAssignNew(ExpanderArrow, SButton)
						.ButtonStyle( FCoreStyle::Get(), "NoBorder" )
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.ClickMethod(EButtonClickMethod::MouseDown)
						.OnClicked(this, &SCoincidenceWidget::OnExpanderClicked)
						.IsFocusable(false)
						.ContentPadding(0.f)
						.ForegroundColor( FSlateColor::UseForeground() )
						[
							SNew(SImage)
							.Image( this, &SCoincidenceWidget::GetExpanderImage)
							.ColorAndOpacity( FSlateColor::UseForeground() )
						]
					]
            				
					+ SHorizontalBox::Slot()
					.Padding(2.f, 0.f, 0.f, 0.f)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(InArgs._StringTablesWithCoincidence->AssetData->AssetName.ToString()))
						.TextStyle(FEditorStyle::Get(), "DetailsView.CategoryTextStyle")
						.Justification(ETextJustify::Left)
					]
				]
			]
		]
		
	];
		
	for (TPair<FString, FString>& Pair : InArgs._StringTablesWithCoincidence->StringMap)
	{
		TSharedPtr<SBox> Box;
		
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
			.Padding(FMargin(2.f, 0.f, 2.f, 1.f))
			[
				SNew(SSplitter)
				.Style(FEditorStyle::Get(), "DetailsView.Splitter")
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
					.OnMouseButtonUp(this, &SCoincidenceWidget::OnElementMouseButtonUp, Pair)
					.OnMouseDoubleClick(this, &SCoincidenceWidget::OnElementMouseDoubleClick, Pair.Key)
					[
						SNew(STextBlock)
						.Margin(FMargin(5.f, 0.f, 0.f, 0.f))
						.Justification(ETextJustify::Left)
						.Text(FText::FromString(Pair.Key.Replace(TEXT("\r\n"), TEXT("[↓↓↓]"))))
						.AutoWrapText(false)
						.WrapTextAt(0)
						.Clipping(EWidgetClipping::ClipToBounds)
					]
				]
				+ SSplitter::Slot()
				.Value(FillCoefficient_1)
				.OnSlotResized(SSplitter::FOnSlotResized::CreateSP(this, &SCoincidenceWidget::OnSlotResize, 1))
				[
					SNew(SBorder)
					.BorderImage(&ElementBorderBrush.Get())
					.VAlign(VAlign_Center)
					.OnMouseButtonUp(this, &SCoincidenceWidget::OnElementMouseButtonUp, Pair)
					.OnMouseDoubleClick(this, &SCoincidenceWidget::OnElementMouseDoubleClick, Pair.Key)
					[
						SNew(STextBlock)
						.Margin(FMargin(5.f, 0.f, 0.f, 0.f))
						.Justification(ETextJustify::Left)
						.Text(FText::FromString(Pair.Value.Replace(TEXT("\r\n"), TEXT("[↓↓↓]"))))
						.AutoWrapText(false)
						.WrapTextAt(0)
						.Clipping(EWidgetClipping::ClipToBounds)
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

	return FEditorStyle::GetBrush(ResourceName);
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

FReply SCoincidenceWidget::OnStringTableMouseDoubleClick(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		TArray<UObject*> AssetsToOpenArray;
		AssetsToOpenArray.Add(AssetData->GetAsset());  
	
		FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get()
		.OpenEditorForAssets(AssetsToOpenArray);
	}
	
	return FReply::Unhandled();
}

FReply SCoincidenceWidget::OnStringTableMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FMenuBuilder MenuBuilder(true, nullptr);
		
		MenuBuilder.BeginSection("AssetContextExploreMenuOptions", FText::FromString("Explore"));
		MenuBuilder.AddMenuEntry(
			FText::FromString("Show in Folder View"),
			FText::FromString("Selects the folder that contains this asset in the Content Browser Sources Panel."),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "ContentBrowser.TabIcon"),
			FUIAction(FExecuteAction::CreateLambda([this]()
			{
				TArray<UObject*> AssetsToFind;
				AssetsToFind.Add(AssetData->GetAsset());  
				FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser").Get().SyncBrowserToAssets(AssetsToFind);
			}))
		);
		MenuBuilder.EndSection();
		
		MenuBuilder.BeginSection("Options", FText::FromString("Options"));
		MenuBuilder.AddMenuEntry(
			FText::FromString("Copy StringTable Name"),
			FText::FromString("Copy the selected StringTable Name"),
			FSlateIcon(FCoreStyle::Get().GetStyleSetName(), "GenericCommands.Copy"),
			FUIAction(FExecuteAction::CreateLambda([this]()
			{
				FPlatformApplicationMisc::ClipboardCopy(*AssetData->AssetName.ToString());
			}))
		);
		
		MenuBuilder.AddMenuEntry(
			FText::FromString("Copy StringTable Reference"),
			FText::FromString("Copy the selected StringTable Reference"),
			FSlateIcon(FCoreStyle::Get().GetStyleSetName(), "GenericCommands.Copy"),
			FUIAction(FExecuteAction::CreateLambda([this]()
			{
				FPlatformApplicationMisc::ClipboardCopy(*FString::Printf(TEXT("%s'%s'"),
					*AssetData->GetClass()->GetName(), *AssetData->ObjectPath.ToString()));
			}))
		);
		MenuBuilder.EndSection();
		
		TSharedRef<SWidget> ContextMenu = MenuBuilder.MakeWidget();
		
		FSlateApplication::Get().PushMenu(
			AsShared(),
			FWidgetPath(),
			ContextMenu,
			MouseEvent.GetScreenSpacePosition(),
			FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
		);

	}
	return FReply::Unhandled();
}

FReply SCoincidenceWidget::OnElementMouseDoubleClick(const FGeometry& Geometry, const FPointerEvent& MouseEvent, FString Key) const
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UStringTable* StringTable = Cast<UStringTable>(AssetData->GetAsset());
		
		if (GEditor)
		{
			UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
			
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(StringTable);
			
			IAssetEditorInstance* EditorInstance = AssetEditorSubsystem->FindEditorForAsset(StringTable, false);

			if (EditorInstance)
			{
				TSharedPtr<SWindow> EditorWindow;
            	if (!EditorWindow.IsValid())
            	{
            		TArray<TSharedRef<SWindow>> AllWindows;
            		FSlateApplication::Get().GetAllVisibleWindowsOrdered(AllWindows);
        
            		for (const TSharedRef<SWindow>& Window : AllWindows)
            		{
            			if (Window->GetTitle().ToString().Contains(AssetData->AssetName.ToString()))
            			{
            				EditorWindow = Window;
            				break;
            			}
            			else
            			{
            				TFunction<void(const TSharedRef<SWidget>&)> FindDockTabsRecursive;
            				
            				FindDockTabsRecursive = [&](const TSharedRef<SWidget>& Widget)
            				{
            					if (Widget->GetTypeAsString() == "SDockTab")
            					{
            						TSharedRef<const SDockTab> DockTab = StaticCastSharedRef<const SDockTab>(Widget);
            						FString TabText = DockTab->GetTabLabel().ToString();

            						UE_LOG(LogTemp, Warning, TEXT("%s"), *TabText );

            						if (TabText.Contains(AssetData->AssetName.ToString()))
            						{
            							UE_LOG(LogTemp, Warning, TEXT("%s"), *DockTab->GetTabLabel().ToString() );
            							EditorWindow = Window;
            							return;
            						}
            					}
            					const FChildren* Children = Widget->GetChildren();
            					if (Children)
            					{
            						for (int32 i = 0; i < Children->Num(); ++i)
            						{
            							TSharedRef<SWidget> Child = ConstCastSharedRef<SWidget>(Children->GetChildAt(i));
            							FindDockTabsRecursive(Child);
            						}
            					}
            				};

            				FindDockTabsRecursive(Window);

            				if (EditorWindow != nullptr) break;
            			}
            		}

            		if (EditorWindow.IsValid())
            		{
            			TSharedPtr<SWidget> FoundSearchBox;
					
            			TFunction<void(TSharedRef<SWidget>)> WidgetSearch = [&](const TSharedRef<SWidget>& Widget)
            			{
            				if (!FoundSearchBox.IsValid() && Widget->GetTypeAsString() == "SSearchBox")
            				{
            					FoundSearchBox = Widget;
            				}
                        
            				if (!FoundSearchBox.IsValid())
            				{
            					FChildren* Children = Widget->GetChildren();
            					for (int32 i = 0; i < Children->Num() && !FoundSearchBox.IsValid(); ++i)
            					{
            						WidgetSearch(Children->GetChildAt(i));
            					}
            				}
            			};

            			WidgetSearch(EditorWindow.ToSharedRef());
					
            			if (FoundSearchBox.IsValid())
            			{
            				if (TSharedPtr<SSearchBox> SearchBox = StaticCastSharedPtr<SSearchBox>(FoundSearchBox))
            				{
            					SearchBox->SetText(FText::FromString(Key));
            				}
            			}
            		}
            	}
			}
		}
	}
	return FReply::Unhandled();
}

FReply SCoincidenceWidget::OnElementMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& MouseEvent,
	 TPair<FString, FString> Pair)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FMenuBuilder MenuBuilder(true, nullptr);
		MenuBuilder.BeginSection("Options", FText::FromString("Options"));

		MenuBuilder.AddMenuEntry(
			FText::FromString("Copy Key"),
			FText::FromString("Copy the selected Key"),
			FSlateIcon(FCoreStyle::Get().GetStyleSetName(), "GenericCommands.Copy"),
			FUIAction(FExecuteAction::CreateLambda([this, Pair]()
			{
				FPlatformApplicationMisc::ClipboardCopy(*Pair.Key);
			}))
		);

		MenuBuilder.AddMenuEntry(
			FText::FromString("Copy Source String"),
			FText::FromString("Copy the selected Source String"),
			FSlateIcon(FCoreStyle::Get().GetStyleSetName(), "GenericCommands.Copy"),
			FUIAction(FExecuteAction::CreateLambda([this, Pair]()
			{
				FPlatformApplicationMisc::ClipboardCopy(*Pair.Value);
			}))
		);

		MenuBuilder.AddMenuEntry(
			FText::FromString("Copy Row Reference"),
			FText::FromString("Copy the selected Row Reference"),
			FSlateIcon(FCoreStyle::Get().GetStyleSetName(), "GenericCommands.Copy"),
			FUIAction(FExecuteAction::CreateLambda([this, Pair]()
			{
				FPlatformApplicationMisc::ClipboardCopy(*FString::Printf(TEXT("LOCTABLE(\"%s\", \"%s\")"),
					*AssetData->ObjectPath.ToString(), *Pair.Key));
			}))
		);

		TSharedRef<SWidget> ContextMenu = MenuBuilder.MakeWidget();
		
		FSlateApplication::Get().PushMenu(
			AsShared(),
			FWidgetPath(),
			ContextMenu,
			MouseEvent.GetScreenSpacePosition(),
			FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
		);

	}
	return FReply::Unhandled();
}