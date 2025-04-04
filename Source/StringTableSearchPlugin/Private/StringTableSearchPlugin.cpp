// Copyright Epic Games, Inc. All Rights Reserved.

#include "StringTableSearchPlugin.h"

#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "SlateWidgets/SearchInStringTablesWidget.h"

#define LOCTEXT_NAMESPACE "FStringTableSearchPluginModule"

void FStringTableSearchPluginModule::StartupModule()
{
	RegisterSearchInStringTables();
}

void FStringTableSearchPluginModule::ShutdownModule()
{
}

void FStringTableSearchPluginModule::RegisterSearchInStringTables()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("SearchInStringTables"),
		FOnSpawnTab::CreateRaw(this, &FStringTableSearchPluginModule::OnSpawnSearchInStringTablesTab))
	.SetDisplayName(FText::FromString(TEXT("Search in StringTables")))
	.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory())
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.Tabs.FindResults"));
}

TSharedRef<SDockTab> FStringTableSearchPluginModule::OnSpawnSearchInStringTablesTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedRef<SDockTab> NewTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.ToolTipText(LOCTEXT("GlobalFindResultsTabTooltip", "Search for a string in all Blueprint assets."));

	TSharedRef<SSearchInStringTablesWidget> Widget = SNew(SSearchInStringTablesWidget)
		.ContainingTab(NewTab);

	NewTab->SetContent(Widget);

	return NewTab;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStringTableSearchPluginModule, StringTableSearchPlugin)