#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"
#include "MaterialValueType.h"

#include "MaterialExpressionRadialUVs.generated.h"

UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionRadialUVs : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY(meta = (RequiredInput = "false", ToolTip = "Defaults to 'ConstCoordinate' if not specified"))
	FExpressionInput Coordinate;
	
	UPROPERTY(meta =(RequiredInput = "false", ToolTip = "Defaults to (0.f,0.f) if not specified"))
	FExpressionInput Center;

	/** Default scale value if not connected */
	UPROPERTY(EditAnywhere, Category = MaterialExpressionBasic, meta = (OverridingInputProperty = "Coordinate"))
	uint8 ConstCoordinate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionBasic, meta = (ShowAsInputPin = "Advanced", OverridingInputProperty = "Center"))
	FVector2D DefaultCenter;
	
	UMaterialExpressionRadialUVs(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// 基础编译方法
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("RadialUVs"));}

	// 提供输入视图，定义输入
	virtual FExpressionInput* GetInput(int32 InputIndex) override;
	virtual FName GetInputName(int32 InputIndex) const override;
	virtual EMaterialValueType GetInputValueType(int32 InputIndex) override;
	
#endif
	//~ End UMaterialExpression Interface
	
};