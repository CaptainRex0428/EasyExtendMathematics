#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"
#include "MaterialValueType.h"

#include "MaterialExpressionSandDissolve.generated.h"

UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionSandDissolve : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput PannerSpeed;
	
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput SparkingSpeed;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput Alpha;
	
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput CenterPos;

	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput RadialScale;

	/** Default scale value if not connected */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionSandDissolve, meta = (ShowAsInputPin = "Advanced"))
	bool bZDepthMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionSandDissolve, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "MoveSpeed"))
	FVector2D DefaultPannerSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionSandDissolve, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "SparkingSpeed"))
	float DefaultSparkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionSandDissolve, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "Alpha"))
	float DefaultAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionSandDissolve, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "RadialScale"))
	FVector2D DefaultRadialScale;

	UPROPERTY()
	TObjectPtr<UTexture2D> DefaultWhiteNoise;
	
	UMaterialExpressionSandDissolve(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// 基础编译方法
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("SandDissolve"));}

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

inline UObject* UMaterialExpressionSandDissolve::GetReferencedTexture() const
{
	return DefaultWhiteNoise.Get();
}