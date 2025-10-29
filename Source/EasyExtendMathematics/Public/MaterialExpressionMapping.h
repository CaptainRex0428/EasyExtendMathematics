#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "MaterialExpressionIO.h"
#include "Materials/MaterialExpression.h"

#include "MaterialExpressionMapping.generated.h"

UCLASS(MinimalAPI, collapsecategories, hidecategories = Object)
class UMaterialExpressionMapping : public UMaterialExpression
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FExpressionInput Vector;

	UPROPERTY()
	FExpressionInput Location;

	UPROPERTY()
	FExpressionInput Rotation;

	UPROPERTY()
	FExpressionInput Scale;

	/** Default location value if not connected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionMapping, meta = (ShowAsInputPin = "Advanced"))
	FVector ConstLocation;

	/** Default rotation value if not connected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionMapping, meta = (ShowAsInputPin = "Advanced"))
	FVector ConstRotation;

	/** Default scale value if not connected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MaterialExpressionMapping, meta = (ShowAsInputPin = "Advanced"))
	FVector ConstScale;
	
	UMaterialExpressionMapping(const FObjectInitializer& ObjectInitializer);

	//~ Begin UMaterialExpression Interface
#if WITH_EDITOR
	// virtual void Build(MIR::FEmitter& Emitter) override;
	virtual int32 Compile(class FMaterialCompiler* Compiler, int32 OutputIndex) override;
	
	virtual void GetCaption(TArray<FString>& OutCaptions) const override;
	virtual void GetExpressionToolTip(TArray<FString>& OutToolTip) override;
	
	virtual FText GetKeywords() const override {return FText::FromString(TEXT("Mapping"));}
	
#endif
	//~ End UMaterialExpression Interface

	
};
