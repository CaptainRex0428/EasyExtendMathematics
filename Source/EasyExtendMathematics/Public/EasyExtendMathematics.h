// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FEasyExtendMathematicsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TObjectPtr<UTexture2D> GetPerlinNoiseTexture() const { return PerlinNoiseTexture2D; }
	
private:
	TObjectPtr<UTexture2D> PerlinNoiseTexture2D;
};
