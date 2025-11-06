// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyExtendMathematics.h"
#include "Interfaces/IPluginManager.h"
#include "EasyExtendMaterialNodeLogger.h"

#define LOCTEXT_NAMESPACE "FEasyExtendMathematicsModule"

template <typename T>
void LoadTexture(TObjectPtr<T> & TextureObject, const TCHAR * FileName)
{
	TextureObject = LoadObject<T>(nullptr, FileName);
	if (TextureObject)
	{
		// 添加到根集合，防止被GC
		TextureObject->AddToRoot();
		
		EEMLogOnly(FString::Printf(L"Successfully loaded %s",FileName));
	}
	else
	{
		EEMLogOnly(FString::Printf(L"Failed to load %s",FileName));
	}
}

void FEasyExtendMathematicsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	const FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("EasyExtendMathematics"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/EEShaders"), PluginShaderDir);	

	LoadTexture<UTexture2D>(PerlinNoiseTexture2D,TEXT("/EasyExtendMathematics/Texture/T_PerlinNoise_Origin.T_PerlinNoise_Origin"));
	LoadTexture<UVolumeTexture>(PerlinNoiseVolumeTexture,TEXT("/EasyExtendMathematics/Texture/VT_PerlinNoise_Origin.VT_PerlinNoise_Origin"));
	LoadTexture<UTexture2D>(WhiteNoiseTexture2D,TEXT("/EasyExtendMathematics/Texture/T_WhiteNoise.T_WhiteNoise"));
	LoadTexture<UTexture2D>(BlueNoiseTexture2D,TEXT("/EasyExtendMathematics/Texture/T_BlueNoise.T_BlueNoise"));
	LoadTexture<UTexture2D>(PinkNoiseTexture2D,TEXT("/EasyExtendMathematics/Texture/T_PinkNoise.T_PinkNoise"));
	LoadTexture<UTexture2D>(VoronoiNoiseTexture2D,TEXT("/EasyExtendMathematics/Texture/T_VoronoiNoise.T_VoronoiNoise"));
	LoadTexture<UTexture2D>(SparseNoiseTexture2D,TEXT("/EasyExtendMathematics/Texture/T_SparseNoise.T_SparseNoise"));
	LoadTexture<UTexture2D>(CloudNoiseTexture2D,TEXT("/EasyExtendMathematics/Texture/T_CloudNoise.T_CloudNoise"));
	LoadTexture<UTexture2D>(HairShiftTexture2D,TEXT("/EasyExtendMathematics/Texture/T_Hair_Shift.T_Hair_Shift"));
}


void FEasyExtendMathematicsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	ResetAllShaderSourceDirectoryMappings();

	ResetAllShaderSourceDirectoryMappings();

	// 只在非关闭流程中清理（比如热重载）
	if (!IsEngineExitRequested() && !GExitPurge)
	{
		if (PerlinNoiseTexture2D && IsValid(PerlinNoiseTexture2D) && PerlinNoiseTexture2D->IsRooted())
		{
			PerlinNoiseTexture2D->RemoveFromRoot();
		}
		
		if (PerlinNoiseVolumeTexture && IsValid(PerlinNoiseVolumeTexture) && PerlinNoiseVolumeTexture->IsRooted())
		{
			PerlinNoiseVolumeTexture->RemoveFromRoot();
		}

		if (WhiteNoiseTexture2D && IsValid(WhiteNoiseTexture2D) && WhiteNoiseTexture2D->IsRooted())
		{
			WhiteNoiseTexture2D->RemoveFromRoot();
		}

		if (BlueNoiseTexture2D && IsValid(BlueNoiseTexture2D) && BlueNoiseTexture2D->IsRooted())
		{
			BlueNoiseTexture2D->RemoveFromRoot();
		}

		if (PinkNoiseTexture2D && IsValid(PinkNoiseTexture2D) && PinkNoiseTexture2D->IsRooted())
		{
			PinkNoiseTexture2D->RemoveFromRoot();
		}

		if (VoronoiNoiseTexture2D && IsValid(VoronoiNoiseTexture2D) && VoronoiNoiseTexture2D->IsRooted())
		{
			VoronoiNoiseTexture2D->RemoveFromRoot();
		}

		if (SparseNoiseTexture2D && IsValid(SparseNoiseTexture2D) && SparseNoiseTexture2D->IsRooted())
		{
			SparseNoiseTexture2D->RemoveFromRoot();
		}

		if (CloudNoiseTexture2D && IsValid(CloudNoiseTexture2D) && CloudNoiseTexture2D->IsRooted())
		{
			CloudNoiseTexture2D->RemoveFromRoot();
		}

		if (HairShiftTexture2D && IsValid(HairShiftTexture2D) && HairShiftTexture2D->IsRooted())
		{
			HairShiftTexture2D->RemoveFromRoot();
		}
	}
	
	PerlinNoiseTexture2D = nullptr;
	PerlinNoiseVolumeTexture = nullptr;
	WhiteNoiseTexture2D = nullptr;
	BlueNoiseTexture2D = nullptr;
	PinkNoiseTexture2D = nullptr;
	VoronoiNoiseTexture2D = nullptr;
	SparseNoiseTexture2D = nullptr;
	CloudNoiseTexture2D = nullptr;
	HairShiftTexture2D = nullptr;
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyExtendMathematicsModule, EasyExtendMathematics)