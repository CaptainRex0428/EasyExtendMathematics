#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"
#include "MaterialValueType.h"

#include "MaterialExpressionBumpOffsetThickness.generated.h"


UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionBumpOffsetThickness : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY(meta = (RequiredInput = "false", ToolTip = ""))
	FExpressionInput Normal;
	
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput View;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput Depth;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput DepthOffset;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput Resolution;

	/** Default scale value if not connected */
	UPROPERTY(EditAnywhere, Category = MaterialExpressionBasic, meta = (OverridingInputProperty = "Depth"))
	float DefaultDepth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionBasic, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "DepthOffset"))
	float DefaultDepthOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionBasic, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "Resolution"))
	float DefaultResolution;

	UPROPERTY()
	TObjectPtr<UTexture2D> DefaultPerlinNoise;
	
	UMaterialExpressionBumpOffsetThickness(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// 基础编译方法
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("BumpOffsetThickness"));}

	// 提供输入视图，定义输入
	virtual FExpressionInput* GetInput(int32 InputIndex) override;
	virtual FName GetInputName(int32 InputIndex) const override;
	virtual EMaterialValueType GetInputValueType(int32 InputIndex) override;

	// 来响应属性变化
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// 贴图引用
	virtual UObject* GetReferencedTexture() const override;
	virtual bool CanReferenceTexture() const override { return true; }
	
#endif
	//~ End UMaterialExpression Interface
	
};

inline UObject* UMaterialExpressionBumpOffsetThickness::GetReferencedTexture() const
{
	return DefaultPerlinNoise.Get();
}