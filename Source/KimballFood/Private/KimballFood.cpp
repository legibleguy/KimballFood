// Copyright Epic Games, Inc. All Rights Reserved.

#include "KimballFood.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif
#include "KimballFoodSettings.h"

#define LOCTEXT_NAMESPACE "FKimballFoodModule"

void FKimballFoodModule::StartupModule()
{
	#if WITH_EDITOR
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule)
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "KimballFood",
			LOCTEXT("KimballFoodSettingsName", "Kimball Food"),
			LOCTEXT("KimballFoodSettingsDescription", "Configure the Kimball Food plugin."),
			GetMutableDefault<UKimballFoodSettings>()
		);
	}
	#endif
}

void FKimballFoodModule::ShutdownModule()
{
	#if WITH_EDITOR
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "KimballFood");
	}
	#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKimballFoodModule, KimballFood)