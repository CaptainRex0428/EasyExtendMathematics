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
	
private:
	TObjectPtr<UTexture2D> PerlinNoiseTexture2D;
	TObjectPtr<UVolumeTexture> PerlinNoiseVolumeTexture;
};
