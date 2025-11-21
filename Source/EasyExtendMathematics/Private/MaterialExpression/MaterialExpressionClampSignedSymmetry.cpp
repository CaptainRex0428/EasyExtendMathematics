#include "MaterialExpression/MaterialExpressionClampSignedSymmetry.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 

#define LOCTEXT_NAMESPACE "UMaterialExpressionClampSignedSymmetry"

UMaterialExpressionClampSignedSymmetry::UMaterialExpressionClampSignedSymmetry(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	DefaultThreshold(1.f)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// 定义所属分类
		FText Category_Display;
		FConstructorStatics()
			: Category_Display(LOCTEXT("Basic", "Basic"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.Category_Display);
#endif
}

#if WITH_EDITOR

int32 UMaterialExpressionClampSignedSymmetry::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{

	if (!Value.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing Value input"));
	}
	int32 ValueInput = Value.Compile(Compiler);

	int32 ThresholdInput = Threshold.GetTracedInput().Expression ? 
		Threshold.Compile(Compiler) : 
		Compiler->Constant(DefaultThreshold);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();

	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Value");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Threshold")});
	
	MaterialExpressionCustom->Code = TEXT(R"(
		return clamp_signed(Value, Threshold);
	)");
	
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float1;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Basic/Math.ush");	
	
	TArray<int32> Inputs = TArray<int32>{ValueInput,ThresholdInput};
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionClampSignedSymmetry::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("ClampSigned(Symmetry)"));
}

void UMaterialExpressionClampSignedSymmetry::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT(""));
}

// this define is only used for the following function
FExpressionInput* UMaterialExpressionClampSignedSymmetry::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Value;
		case 1: return &Threshold;
		default: return nullptr;
	}
}

FName UMaterialExpressionClampSignedSymmetry::GetInputName(int32 InputIndex) const
{
	
	switch (InputIndex)
	{
	case 0: return TEXT("Value");
	case 1: return TEXT("Threshold");
	default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionClampSignedSymmetry::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0: return MCT_Float;
	case 1: return MCT_Float;
	default: return MCT_Unknown;
	}
}

bool UMaterialExpressionClampSignedSymmetry::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

void UMaterialExpressionClampSignedSymmetry::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif
