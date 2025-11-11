#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"
#include "Materials/MaterialExpressionTextureBase.h"
#include "MaterialValueType.h"

#include "MaterialExpressionCurlNoise2D.generated.h"

UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionCurlNoise2D : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY(meta = (RequiredInput = "false", ToolTip = "Defaults to 'ConstCoordinate' if not specified"))
	FExpressionInput Coordinate;
	
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to 'PerlinNoise' if not specified"))
	FExpressionInput TextureObject;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput SampleOffset;

	/** Default scale value if not connected */
	UPROPERTY(EditAnywhere, Category = MaterialExpressionTextureSample, meta = (OverridingInputProperty = "Coordinate"))
	uint8 ConstCoordinate;

	UPROPERTY(EditAnywhere, Category = MaterialExpressionTextureSample, meta = (ShowAsInputPin = "Advanced"))
	FVector2D Scale;

	UPROPERTY(EditAnywhere, Category = MaterialExpressionTextureSample, meta = (ShowAsInputPin = "Advanced"))
	FVector2D Offset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionCurlNoise, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "SampleOffset"))
	float DefaultSampleOffset;

	UPROPERTY(EditAnywhere, Category = MaterialExpressionTextureSample, meta = (ShowAsInputPin = "Advanced"))
	bool SymmetrySample;

	UPROPERTY()
	TObjectPtr<UTexture2D> DefaultNoiseTexture;
	
	UMaterialExpressionCurlNoise2D(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// 基础编译方法
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("CurlNoise2D"));}

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

inline UObject* UMaterialExpressionCurlNoise2D::GetReferencedTexture() const
{
	return DefaultNoiseTexture.Get();
}
