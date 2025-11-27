#include "MaterialExpression/Shape/MaterialExpressionEllipseArcFixed.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 

#define LOCTEXT_NAMESPACE "UMatetialExpressionEllipseArcFixed"

UMaterialExpressionEllipseArcFixed::UMaterialExpressionEllipseArcFixed(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	DefaultAxisLength(0.5f)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// 定义所属分类
		FText Category_Display;
		FConstructorStatics()
			: Category_Display(LOCTEXT("Shape", "Shape"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.Category_Display);
#endif
}

#if WITH_EDITOR

int32 UMaterialExpressionEllipseArcFixed::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if (!Ramp.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing Ramp input"));
	}
	int32 RampInput = Ramp.Compile(Compiler);

	int32 AxisLengthInput = AxisLength.GetTracedInput().Expression ? 
		AxisLength.Compile(Compiler) : 
		Compiler->Constant(DefaultAxisLength);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();

	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Ramp");
	MaterialExpressionCustom->Inputs.Add({ TEXT("AxisLength")});
	
	MaterialExpressionCustom->Code = TEXT(R"(
		return EllipseArcFixed(Ramp, AxisLength);
	)");
	
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float1;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Shape/Ellipse.ush");	
	
	TArray<int32> Inputs = TArray<int32>{RampInput,AxisLengthInput};
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionEllipseArcFixed::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("EllipseArcFixed"));
}

void UMaterialExpressionEllipseArcFixed::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT(""));
}

// this define is only used for the following function
FExpressionInput* UMaterialExpressionEllipseArcFixed::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Ramp;
		case 1: return &AxisLength;
		default: return nullptr;
	}
}

FName UMaterialExpressionEllipseArcFixed::GetInputName(int32 InputIndex) const
{
	
	switch (InputIndex)
	{
	case 0: return TEXT("Ramp");
	case 1: return TEXT("AxisLength");
	default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionEllipseArcFixed::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0: return MCT_Float;
	case 1: return MCT_Float;
	default: return MCT_Unknown;
	}
}

bool UMaterialExpressionEllipseArcFixed::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

void UMaterialExpressionEllipseArcFixed::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif
