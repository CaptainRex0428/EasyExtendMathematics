#include "MaterialExpressionCurlNoise3D.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 
#include "LandscapeRender.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionCurlNoise3D"

UMaterialExpressionCurlNoise3D::UMaterialExpressionCurlNoise3D(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	Scale(1.f,1.f),
	Offset(0.f,0.f),
	DefaultSampleOffset(0.0001f),
	SymmetrySample(false),
	DefaultNoiseTexture(nullptr)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// 定义所属分类
		FText Category_Display;
		FConstructorStatics()
			: Category_Display(LOCTEXT("CurlNoise", "CurlNoise"))
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
		DefaultNoiseTexture = Module.GetPerlinNoiseTexture();
	}
}

#if WITH_EDITOR

int32 UMaterialExpressionCurlNoise3D::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if (!TextureObject.GetTracedInput().Expression && !DefaultNoiseTexture)
	{
		return Compiler->Errorf(TEXT("Missing NoiseTexture input"));
	}

	int32 TextureCodeIndex = TextureObject.GetTracedInput().Expression ? TextureObject.Compile(Compiler) : Compiler->Texture(DefaultNoiseTexture,SAMPLERTYPE_Masks);

	// Coordinate 输入处理
	int32 CoordinateIdx = Coordinate.GetTracedInput().Expression ? Coordinate.Compile(Compiler) : Compiler->TextureCoordinate(ConstCoordinate, false, false);
	
	if (CoordinateIdx == INDEX_NONE)
	{
		return Compiler->Errorf(TEXT("Coordinate compiled error."));
	}

	// 处理SampleOffset输入（如果没有连接则使用常量）
	int32 SampleOffsetInput = SampleOffset.GetTracedInput().Expression ? 
		SampleOffset.Compile(Compiler) : 
		Compiler->Constant(DefaultSampleOffset);

	// 处理UVs输入的位移和缩放
	int32 CoordIdx = Compiler->Add(Compiler->Mul(CoordinateIdx,Compiler->Constant2(Scale.X,Scale.Y)),Compiler->Constant2(Offset.X,Offset.Y));
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	
	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Texture");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Coordinate") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("SampleOffset") });
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Curl.ush");

	if (SymmetrySample)
	{
		MaterialExpressionCustom->Code = TEXT(R"(
		return CurlNoise3D_Central(Texture, TextureSampler, Coordinate, SampleOffset);
		)");
	}
	else
	{
		MaterialExpressionCustom->Code = TEXT(R"(
		return CurlNoise3D(Texture, TextureSampler, Coordinate, SampleOffset);
		)");
	}
	
	
	TArray<int32> Inputs{ TextureCodeIndex, CoordIdx, SampleOffsetInput };
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionCurlNoise3D::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("CurlNoise3D"));
}

void UMaterialExpressionCurlNoise3D::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Output is a 3D vector field distorted by curl noise."));
	OutToolTip.Add(TEXT("Uses texture sampling to compute the curl of a noise field."));
	OutToolTip.Add(TEXT("Z channel is calculated by sampling ‘x * 0.7’ "));
}

FExpressionInput* UMaterialExpressionCurlNoise3D::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Coordinate;
		case 1: return &TextureObject;
		case 2: return &SampleOffset;
		default: return nullptr;
	}
	
}

FName UMaterialExpressionCurlNoise3D::GetInputName(int32 InputIndex) const
{
	switch (InputIndex)
	{
		case 0: return TEXT("Coordinate");
		case 1: return TEXT("Texture"); 
		case 2: return TEXT("SampleOffset");
		default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionCurlNoise3D::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0:  // Coordinate
		return MCT_Float2;  // UV 坐标是 Float2
			
	case 1:  // TextureObject - 这就是你要找的！
		return MCT_Texture2D;  // ✅ 纹理类型！这就是解决连接问题的关键！
			
	case 2:  // SampleOffset
		return MCT_Float1;  // SampleOffset 是 Float1
			
	default:
		return MCT_Unknown;
	}
}

#endif
