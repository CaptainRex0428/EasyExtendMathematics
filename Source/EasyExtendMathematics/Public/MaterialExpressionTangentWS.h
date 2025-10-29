#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"

#include "MaterialExpressionTangentWS.generated.h"

UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionTangentWS : public UMaterialExpression
{

	GENERATED_BODY()

public:
	UPROPERTY()
	FExpressionInput NormalWS;

	UPROPERTY()
	FExpressionInput TangentRotate;

	UPROPERTY(EditAnywhere, Category = MaterialExpressionTangent, meta = (ShowAsInputPin = "Advanced"))
	bool Obverse = false;
	
	UMaterialExpressionTangentWS(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// virtual void Build(MIR::FEmitter& Emitter) override;
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("TangentWS"));}
	
#endif
	//~ End UMaterialExpression Interface
	
};
