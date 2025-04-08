// Copyright (c) 2025 Alexander "gradient92" Vashchenko
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "Modules/ModuleManager.h"

class FStringTableSearchPluginModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void RegisterSearchInStringTables();

	TSharedRef<SDockTab> OnSpawnSearchInStringTablesTab(const FSpawnTabArgs& SpawnTabArgs);
	TSharedPtr<SDockTab> ConstructedDockTab;
};
