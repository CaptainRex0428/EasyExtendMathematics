#include "MaterialExpressionCurlNoise2D.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 
#include "LandscapeRender.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionCurlNoise2D"

UMaterialExpressionCurlNoise2D::UMaterialExpressionCurlNoise2D(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	DefaultSampleOffset(0.0001f),
	DefaultNoiseTexture(nullptr)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_Display;
		FConstructorStatics()
			: NAME_Display(LOCTEXT("CurlNoise2D", "CurlNoise2D"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_Display);
#endif

	if (FModuleManager::Get().IsModuleLoaded("EasyExtendMathematics"))
	{
		FEasyExtendMathematicsModule& Module = FModuleManager::GetModuleChecked<FEasyExtendMathematicsModule>("EasyExtendMathematics");
		DefaultNoiseTexture = Module.GetPerlinNoiseTexture();
	}
}

#if WITH_EDITOR

int32 UMaterialExpressionCurlNoise2D::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
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

	// 处理Scale输入（如果没有连接则使用常量）
	int32 SampleOffsetInput = SampleOffset.GetTracedInput().Expression ? 
		SampleOffset.Compile(Compiler) : 
		Compiler->Constant(DefaultSampleOffset);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	
	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Texture");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Coordinate") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("SampleOffset") });
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float2;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Curl.ush");

	MaterialExpressionCustom->Code = TEXT(R"(
		return CurlNoise2D(Texture, TextureSampler, Coordinate, SampleOffset);
		)");
	
	TArray<int32> Inputs{ TextureCodeIndex, CoordinateIdx, SampleOffsetInput };
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionCurlNoise2D::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("CurlNoise2D"));
}

void UMaterialExpressionCurlNoise2D::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Output is a 2D vector field distorted by curl noise."));
	OutToolTip.Add(TEXT("Uses texture sampling to compute the curl of a noise field."));
}

FExpressionInput* UMaterialExpressionCurlNoise2D::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Coordinate;
		case 1: return &TextureObject;
		case 2: return &SampleOffset;
		default: return nullptr;
	}
	
}

FName UMaterialExpressionCurlNoise2D::GetInputName(int32 InputIndex) const
{
	switch (InputIndex)
	{
		case 0: return TEXT("Coordinate");
		case 1: return TEXT("Texture"); 
		case 2: return TEXT("SampleOffset");
		default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionCurlNoise2D::GetInputValueType(int32 InputIndex)
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
