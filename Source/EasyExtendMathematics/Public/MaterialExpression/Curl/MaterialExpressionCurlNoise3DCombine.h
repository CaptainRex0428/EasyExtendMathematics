#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"
#include "MaterialValueType.h"
#include "Materials/MaterialExpressionWorldPosition.h" 

#include "MaterialExpressionCurlNoise3DCombine.generated.h"

UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionCurlNoise3DCombine : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY(meta = (RequiredInput = "false", ToolTip = "Defaults to 'ConstCoordinate' if not specified"))
	FExpressionInput Position;
	
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput TextureObject;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput ScaleX;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput OffsetX;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput ScaleY;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput OffsetY;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput ScaleZ;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput OffsetZ;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput SampleOffset;

	/** Default scale value if not connected */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced"))
	TEnumAsByte<EWorldPositionIncludedOffsets> WorldPositionOffsetInclude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced"))
	float PositionScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "ScaleX"))
	FVector2D DefaultScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "OffsetX"))
	FVector2D DefaultOffsetX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "ScaleY"))
	FVector2D DefaultScaleY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "OffsetY"))
	FVector2D DefaultOffsetY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "ScaleZ"))
	FVector2D DefaultScaleZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "OffsetZ"))
	FVector2D DefaultOffsetZ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "SampleOffset"))
	float DefaultSampleOffset;

	UPROPERTY(EditAnywhere, Category = MaterialExpressionTextureSample, meta = (ShowAsInputPin = "Advanced"))
	bool SymmetrySample;

	UPROPERTY()
	TObjectPtr<UTexture2D> DefaultNoiseTexture;
	
	UMaterialExpressionCurlNoise3DCombine(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// 基础编译方法
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("CurlNoise3DCombine"));}

	// 提供输入视图，定义输入
	virtual FExpressionInput* GetInput(int32 InputIndex) override;
	virtual FName GetInputName(int32 InputIndex) const override;
	virtual EMaterialValueType GetInputValueType(int32 InputIndex) override;

	// 贴图引用
	virtual UObject* GetReferencedTexture() const override;
	virtual bool CanReferenceTexture() const override { return true; }
	
#endif
	//~ End UMaterialExpression Interface
	
};

inline UObject* UMaterialExpressionCurlNoise3DCombine::GetReferencedTexture() const
{
	return DefaultNoiseTexture.Get();
}
