#include "MaterialExpressionCurlNoise3DCombine.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 

#define LOCTEXT_NAMESPACE "UMaterialExpressionCurlNoise3DCombine"

UMaterialExpressionCurlNoise3DCombine::UMaterialExpressionCurlNoise3DCombine(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	WorldPositionOffsetInclude(WPT_Default),
	PositionScale(0.01),
	DefaultScaleX(1.f,1.f),
	DefaultOffsetX(0.f,0.f),
	DefaultScaleY(1.f,1.f),
	DefaultOffsetY(0.f,0.f),
	DefaultScaleZ(1.f,1.f),
	DefaultOffsetZ(0.f,0.f),
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

int32 UMaterialExpressionCurlNoise3DCombine::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if (!TextureObject.GetTracedInput().Expression && !DefaultNoiseTexture)
	{
		return Compiler->Errorf(TEXT("Missing NoiseTexture input"));
	}

	int32 TextureCodeIndex = TextureObject.GetTracedInput().Expression ? TextureObject.Compile(Compiler) : Compiler->Texture(DefaultNoiseTexture,SAMPLERTYPE_Masks);

	// Coordinate 输入处理
	int32 PosInputIdx = Position.GetTracedInput().Expression ? Position.Compile(Compiler) : Compiler->WorldPosition(WorldPositionOffsetInclude);
	
	if (PosInputIdx == INDEX_NONE)
	{
		return Compiler->Errorf(TEXT("Position compiled error."));
	}

	int32 PosOutputIdx = Compiler->Mul(PosInputIdx,Compiler->Constant(PositionScale));
	
	int32 ScaleXInput = ScaleX.GetTracedInput().Expression ? 
		ScaleX.Compile(Compiler) : 
		Compiler->Constant2(DefaultScaleX.X,DefaultScaleX.Y);

	int32 ScaleYInput = ScaleY.GetTracedInput().Expression ? 
		ScaleY.Compile(Compiler) : 
		Compiler->Constant2(DefaultScaleY.X,DefaultScaleY.Y);

	int32 ScaleZInput = ScaleZ.GetTracedInput().Expression ? 
		ScaleZ.Compile(Compiler) : 
		Compiler->Constant2(DefaultScaleZ.X,DefaultScaleZ.Y);

	int32 OffsetXInput = OffsetX.GetTracedInput().Expression ? 
		OffsetX.Compile(Compiler) : 
		Compiler->Constant2(DefaultOffsetX.X,DefaultOffsetX.Y);

	int32 OffsetYInput = OffsetY.GetTracedInput().Expression ? 
		OffsetY.Compile(Compiler) : 
		Compiler->Constant2(DefaultOffsetY.X,DefaultOffsetY.Y);

	int32 OffsetZInput = OffsetZ.GetTracedInput().Expression ? 
		OffsetZ.Compile(Compiler) : 
		Compiler->Constant2(DefaultOffsetZ.X,DefaultOffsetZ.Y);
	
	// 处理SampleOffset输入（如果没有连接则使用常量）
	int32 SampleOffsetInput = SampleOffset.GetTracedInput().Expression ? 
		SampleOffset.Compile(Compiler) : 
		Compiler->Constant(DefaultSampleOffset);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	
	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Texture");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Position") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("ScaleX") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("OffsetX") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("ScaleY") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("OffsetY") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("ScaleZ") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("OffsetZ") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("SampleOffset") });
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Curl.ush");

	if (SymmetrySample)
	{
		MaterialExpressionCustom->Code = TEXT(R"(
		return CurlNoise3D_Combine_Central(Texture, TextureSampler, Position, ScaleX, OffsetX, ScaleY, OffsetY, ScaleZ, OffsetZ, SampleOffset);
		)");
	}
	else
	{
		MaterialExpressionCustom->Code = TEXT(R"(
		return CurlNoise3D_Combine(Texture, TextureSampler, Position, ScaleX, OffsetX, ScaleY, OffsetY, ScaleZ, OffsetZ, SampleOffset);
		)");
	}
	
	
	TArray<int32> Inputs{ TextureCodeIndex, PosOutputIdx, ScaleXInput, OffsetXInput, ScaleYInput, OffsetYInput, ScaleZInput, OffsetZInput, SampleOffsetInput };
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionCurlNoise3DCombine::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("CurlNoise3DCombine"));
}

void UMaterialExpressionCurlNoise3DCombine::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Output is a 3D vector field distorted by curl noise."));
	OutToolTip.Add(TEXT("Uses texture sampling to compute the curl of a noise field."));
	OutToolTip.Add(TEXT("Z channel is calculated by sampling ‘x * 0.7’ "));
}

FExpressionInput* UMaterialExpressionCurlNoise3DCombine::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Position;
		case 1: return &TextureObject;
		case 2: return &ScaleX;
		case 3: return &OffsetX;
		case 4: return &ScaleY;
		case 5: return &OffsetY;
		case 6: return &ScaleZ;
		case 7: return &OffsetZ;
		case 8: return &SampleOffset;
		default: return nullptr;
	}
	
}

FName UMaterialExpressionCurlNoise3DCombine::GetInputName(int32 InputIndex) const
{
	switch (InputIndex)
	{
		case 0: return TEXT("Pos");
		case 1: return TEXT("Texture"); 
		case 2: return TEXT("ScaleX");
		case 3: return TEXT("OffsetX");
		case 4: return TEXT("ScaleY");
		case 5: return TEXT("OffsetY");
		case 6: return TEXT("ScaleZ");
		case 7: return TEXT("OffsetZ");
		case 8: return TEXT("SampleOffset");
		default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionCurlNoise3DCombine::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0: return MCT_Float3;
	case 1: return MCT_Texture2D; 
	case 2: return MCT_Float2;
	case 3: return MCT_Float2;
	case 4: return MCT_Float2;
	case 5: return MCT_Float2;
	case 6: return MCT_Float2;
	case 7: return MCT_Float2;
	case 8: return MCT_Float1;
			
	default:
		return MCT_Unknown;
	}
}

#endif
