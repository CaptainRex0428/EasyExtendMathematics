#include "MaterialExpression/MaterialExpressionOffsetRamp.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 

#define LOCTEXT_NAMESPACE "UMatetialExpressionOffsetRamp"

UMaterialExpressionOffsetRamp::UMaterialExpressionOffsetRamp(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	DefaultOffset(0.f)
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

int32 UMaterialExpressionOffsetRamp::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{

	if (!Ramp.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing Ramp input"));
	}
	int32 RampInput = Ramp.Compile(Compiler);

	int32 OffsetInput = Offset.GetTracedInput().Expression ? 
		Offset.Compile(Compiler) : 
		Compiler->Constant(DefaultOffset);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();

	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Ramp");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Offset")});
	
	MaterialExpressionCustom->Code = TEXT(R"(
		return offsetRamp(Ramp, Offset);
	)");
	
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float1;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Basic/Math.ush");	
	
	TArray<int32> Inputs = TArray<int32>{RampInput,OffsetInput};
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionOffsetRamp::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("OffsetRamp"));
}

void UMaterialExpressionOffsetRamp::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT(""));
}

// this define is only used for the following function
FExpressionInput* UMaterialExpressionOffsetRamp::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Ramp;
		case 1: return &Offset;
		default: return nullptr;
	}
}

FName UMaterialExpressionOffsetRamp::GetInputName(int32 InputIndex) const
{
	
	switch (InputIndex)
	{
	case 0: return TEXT("Ramp");
	case 1: return TEXT("Offset");
	default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionOffsetRamp::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0: return MCT_Float;
	case 1: return MCT_Float;
	default: return MCT_Unknown;
	}
}

bool UMaterialExpressionOffsetRamp::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

void UMaterialExpressionOffsetRamp::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif
