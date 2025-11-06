// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#include "Engine/VolumeTexture.h"

class FEasyExtendMathematicsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TObjectPtr<UTexture2D> GetPerlinNoiseTexture() const { return PerlinNoiseTexture2D; }
	TObjectPtr<UVolumeTexture> GetPerlinNoiseVolumeTexture() const { return PerlinNoiseVolumeTexture; }
	TObjectPtr<UTexture2D> GetWhiteNoiseTexture() const { return WhiteNoiseTexture2D; }
	TObjectPtr<UTexture2D> GetBlueNoiseTexture() const { return BlueNoiseTexture2D; }
	TObjectPtr<UTexture2D> GetPinkNoiseTexture() const { return PinkNoiseTexture2D; }
	TObjectPtr<UTexture2D> GetVoronoiNoiseTexture() const { return VoronoiNoiseTexture2D; }
	TObjectPtr<UTexture2D> GetSparseNoiseTexture2D() const { return SparseNoiseTexture2D; }
	TObjectPtr<UTexture2D> GetCloudNoiseTexture2D() const { return CloudNoiseTexture2D; }
	TObjectPtr<UTexture2D> GetHairShiftTexture2D() const { return HairShiftTexture2D; }
	
private:
	TObjectPtr<UTexture2D> PerlinNoiseTexture2D;
	TObjectPtr<UVolumeTexture> PerlinNoiseVolumeTexture;

	TObjectPtr<UTexture2D> WhiteNoiseTexture2D;
	TObjectPtr<UTexture2D> BlueNoiseTexture2D;
	TObjectPtr<UTexture2D> PinkNoiseTexture2D;
	TObjectPtr<UTexture2D> VoronoiNoiseTexture2D;
	TObjectPtr<UTexture2D> SparseNoiseTexture2D;
	TObjectPtr<UTexture2D> CloudNoiseTexture2D;
	
	TObjectPtr<UTexture2D> HairShiftTexture2D;
};
