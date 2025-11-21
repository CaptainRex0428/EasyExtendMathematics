#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"
#include "MaterialValueType.h"

#include "MaterialExpressionClampSignedSymmetry.generated.h"


UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionClampSignedSymmetry : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY(meta = (RequiredInput = "true", ToolTip = ""))
	FExpressionInput Value;
	
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = ""))
	FExpressionInput Threshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionBasic, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "Threshold"))
	float DefaultThreshold;
	
	UMaterialExpressionClampSignedSymmetry(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// 基础编译方法
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("ClampSignedSymmetry"));}

	// 提供输入视图，定义输入
	virtual FExpressionInput* GetInput(int32 InputIndex) override;
	virtual FName GetInputName(int32 InputIndex) const override;
	virtual EMaterialValueType GetInputValueType(int32 InputIndex) override;

	// 来响应属性变化
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif
	//~ End UMaterialExpression Interface
	
};