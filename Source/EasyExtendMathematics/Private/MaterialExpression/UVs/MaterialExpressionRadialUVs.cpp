#include "MaterialExpression/UVs/MaterialExpressionRadialUVs.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 
#include "LandscapeRender.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionCurlNoise2D"

UMaterialExpressionRadialUVs::UMaterialExpressionRadialUVs(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	ConstCoordinate(0),
	DefaultCenter(0.5f,0.5f)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// 定义所属分类
		FText Category_Display;
		FConstructorStatics()
			: Category_Display(LOCTEXT("Coord", "Coord"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.Category_Display);
#endif
}

#if WITH_EDITOR

int32 UMaterialExpressionRadialUVs::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	// Coordinate 输入处理
	int32 CoordinateIdx = Coordinate.GetTracedInput().Expression ? Coordinate.Compile(Compiler) : Compiler->TextureCoordinate(ConstCoordinate, false, false);
	
	if (CoordinateIdx == INDEX_NONE)
	{
		return Compiler->Errorf(TEXT("Coordinate compiled error."));
	}

	// 处理SampleOffset输入（如果没有连接则使用常量）
	int32 CenterInput = Center.GetTracedInput().Expression ? 
		Center.Compile(Compiler) : 
		Compiler->Constant2(DefaultCenter.X,DefaultCenter.Y);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	
	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Coordinate");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Center") });
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float2;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/UVs/CustomUVs.ush");

	
		MaterialExpressionCustom->Code = TEXT(R"(
		return RadialUVs(Coordinate, Center);
		)");
	
	
	
	TArray<int32> Inputs{ CoordinateIdx, CenterInput };
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionRadialUVs::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("RadialUVs"));
}

void UMaterialExpressionRadialUVs::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Output is a 2D vector field in radial mode from flat mode."));
}

FExpressionInput* UMaterialExpressionRadialUVs::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Coordinate;
		case 1: return &Center;
		default: return nullptr;
	}
	
}

FName UMaterialExpressionRadialUVs::GetInputName(int32 InputIndex) const
{
	switch (InputIndex)
	{
		case 0: return TEXT("Coordinate");
		case 1: return TEXT("Center"); 
		default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionRadialUVs::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0:
		return MCT_Float2;
			
	case 1:
		return MCT_Float2;
			
	default:
		return MCT_Unknown;
	}
}

#endif
