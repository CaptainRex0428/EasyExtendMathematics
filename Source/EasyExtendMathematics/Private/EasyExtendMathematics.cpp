// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyExtendMathematics.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FEasyExtendMathematicsModule"

void FEasyExtendMathematicsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	const FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("EasyExtendMathematics"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/EEShaders"), PluginShaderDir);	
	
}

void FEasyExtendMathematicsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	ResetAllShaderSourceDirectoryMappings();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyExtendMathematicsModule, EasyExtendMathematics)