#include "MaterialExpressionSandDissolve.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 
#include "LandscapeRender.h"
#include "Components/DynamicEntryBoxBase.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionCurlNoise2D"

UMaterialExpressionSandDissolve::UMaterialExpressionSandDissolve(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	bZDepthMove(false)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// 定义所属分类
		FText Category_Display;
		FConstructorStatics()
			: Category_Display(LOCTEXT("Dissolve", "Dissolve"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.Category_Display);
#endif

	if (FModuleManager::Get().IsModuleLoaded("EasyExtendMathematics"))
	{
		FEasyExtendMathematicsModule& Module = FModuleManager::GetModuleChecked<FEasyExtendMathematicsModule>("EasyExtendMathematics");
		DefaultWhiteNoise = Module.GetWhiteNoiseTexture();
	}
	
}

#if WITH_EDITOR

int32 UMaterialExpressionSandDissolve::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if (!DefaultWhiteNoise)
	{
		return Compiler->Errorf(TEXT("Missing White Noise texture input"));
	}
	
	int32 TextureIdx = Compiler->Texture(DefaultWhiteNoise,SAMPLERTYPE_Masks);
	
	// Coordinate 输入处理
	int32 CoordinateIdx = Coordinate.GetTracedInput().Expression ? Coordinate.Compile(Compiler) : Compiler->TextureCoordinate(0, false, false);
	
	if (CoordinateIdx == INDEX_NONE)
	{
		return Compiler->Errorf(TEXT("Coordinate compiled error."));
	}

	
	// UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	//
	// MaterialExpressionCustom->Inputs[0].InputName = TEXT("Coordinate");
	// MaterialExpressionCustom->Inputs.Add({ TEXT("Center") });
	//
	// MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float2;
	//
	// MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Shape.ush");
	//
	//
	// 	MaterialExpressionCustom->Code = TEXT(R"(
	// 	return RadialUVs(Coordinate, Center);
	// 	)");
	//
	//
	//
	// TArray<int32> Inputs{ CoordinateIdx, CenterInput };
	
	// return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);

	return Compiler->Constant(1);
}

void UMaterialExpressionSandDissolve::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("SandDissolve"));
}

void UMaterialExpressionSandDissolve::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Output is a sand dissolve simulation result."));
}

// this define is only used for the following function
FExpressionInput* UMaterialExpressionSandDissolve::GetInput(int32 InputIndex)
{
	if (bZDepthMove)
	{
		switch (InputIndex)
		{
		case 0: return &Coordinate;
		case 1: return &CenterPos;
		case 2: return &RadialScale;
		default: return nullptr;
		}
	}
	else
	{
		switch (InputIndex)
		{
		case 0: return &Coordinate;
		default: return nullptr;
		}
	}
	
	
}

FName UMaterialExpressionSandDissolve::GetInputName(int32 InputIndex) const
{
	if (bZDepthMove)
	{
		switch (InputIndex)
		{
		case 0: return TEXT("Coordinate");
		case 1: return TEXT("CenterPose");
		case 2: return TEXT("RadialScale");
		default: return NAME_None;
		}
	}
	else
	{
		switch (InputIndex)
		{
		case 0: return TEXT("Coordinate");
		default: return NAME_None;
		}
	}
}

EMaterialValueType UMaterialExpressionSandDissolve::GetInputValueType(int32 InputIndex)
{
	if (bZDepthMove)
	{
		switch (InputIndex)
		{
		case 0: return MCT_Float2;
		case 1: return MCT_Float2;
		case 2: return MCT_Float2;
		default: return MCT_Unknown;
		}
	}
	else
	{
		switch (InputIndex)
		{
		case 0: return MCT_Float2;
		default: return MCT_Unknown;
		}
	}
}

bool UMaterialExpressionSandDissolve::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

void UMaterialExpressionSandDissolve::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 当 bZDepthMove 属性改变时，刷新节点
	if (PropertyChangedEvent.Property && 
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UMaterialExpressionSandDissolve, bZDepthMove))
	{
		// 通知材质编辑器重建节点
		if (GraphNode)
		{
			GraphNode->ReconstructNode();
		}
	}
}

#endif
