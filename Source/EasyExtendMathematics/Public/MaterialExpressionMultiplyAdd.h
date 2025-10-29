#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"

#include "MaterialExpressionMultiplyAdd.generated.h"

UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionMultiplyAdd : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY()
	FExpressionInput Input;

	/** 默认乘数值 */
	UPROPERTY(EditAnywhere, Category = MaterialExpressionMultiplyAdd, meta = (ShowAsInputPin = "Advanced"))
	float Multiply;

	/** 默认加数值 */
	UPROPERTY(EditAnywhere, Category = MaterialExpressionMultiplyAdd, meta = (ShowAsInputPin = "Advanced"))
	float Add;

	UMaterialExpressionMultiplyAdd(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// virtual void Build(MIR::FEmitter& Emitter) override;
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("muladd multiply add scale offset"));}
	
#endif
	//~ End UMaterialExpression Interface
	
};
