// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyExtendMathematics.h"
#include "Interfaces/IPluginManager.h"
#include "EasyExtendMaterialNodeLogger.h"

#define LOCTEXT_NAMESPACE "FEasyExtendMathematicsModule"

void FEasyExtendMathematicsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	const FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("EasyExtendMathematics"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/EEShaders"), PluginShaderDir);	

	PerlinNoiseTexture2D = LoadObject<UTexture2D>(nullptr, 
		TEXT("/EasyExtendMathematics/Texture/T_PerlinNoise_Origin.T_PerlinNoise_Origin"));
	
	if (PerlinNoiseTexture2D)
	{
		// 添加到根集合，防止被GC
		PerlinNoiseTexture2D->AddToRoot();
		
		EEMLogOnly("Successfully loaded Perlin Noise texture");
	}
	else
	{
		EEMLogOnly("Failed to load Perlin Noise texture");
	}

	PerlinNoiseVolumeTexture = LoadObject<UVolumeTexture>(nullptr,
		TEXT("/EasyExtendMathematics/Texture/VT_PerlinNoise_Origin.VT_PerlinNoise_Origin"));
	
	if (PerlinNoiseVolumeTexture)
	{
		// 添加到根集合，防止被GC
		PerlinNoiseVolumeTexture->AddToRoot();
		
		EEMLogOnly("Successfully loaded Perlin Noise volume texture");
	}
	else
	{
		EEMLogOnly("Failed to load Perlin Noise volume texture");
	}
}


void FEasyExtendMathematicsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	ResetAllShaderSourceDirectoryMappings();

	ResetAllShaderSourceDirectoryMappings();

	// 只在非关闭流程中清理（比如热重载）
	if (!GIsRequestingExit && !GExitPurge)
	{
		if (PerlinNoiseTexture2D && IsValid(PerlinNoiseTexture2D) && PerlinNoiseTexture2D->IsRooted())
		{
			PerlinNoiseTexture2D->RemoveFromRoot();
		}
		
		if (PerlinNoiseVolumeTexture && IsValid(PerlinNoiseVolumeTexture) && PerlinNoiseVolumeTexture->IsRooted())
		{
			PerlinNoiseVolumeTexture->RemoveFromRoot();
		}
	}
	
	PerlinNoiseTexture2D = nullptr;
	PerlinNoiseVolumeTexture = nullptr;
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyExtendMathematicsModule, EasyExtendMathematics)