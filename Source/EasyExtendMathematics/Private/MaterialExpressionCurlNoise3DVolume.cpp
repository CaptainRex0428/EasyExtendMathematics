#include "MaterialExpressionCurlNoise3DVolume.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 
#include "LandscapeRender.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionCurlNoise3DVolume"

UMaterialExpressionCurlNoise3DVolume::UMaterialExpressionCurlNoise3DVolume(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	PositionScale(0.01),
	DefaultSampleOffset(0.0001f),
	SymmetrySample(false),
	DefaultNoiseVolumeTexture(nullptr)
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
		DefaultNoiseVolumeTexture = Module.GetPerlinNoiseVolumeTexture();
	}
}

#if WITH_EDITOR

int32 UMaterialExpressionCurlNoise3DVolume::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if (!VolumeTextureObject.GetTracedInput().Expression && !DefaultNoiseVolumeTexture)
	{
		return Compiler->Errorf(TEXT("Missing noise volume texture input"));
	}

	int32 TextureCodeIndex = VolumeTextureObject.GetTracedInput().Expression ? VolumeTextureObject.Compile(Compiler) : Compiler->Texture(DefaultNoiseVolumeTexture,SAMPLERTYPE_Masks);

	// Coordinate 输入处理
	int32 PosInputIdx = Position.GetTracedInput().Expression ? Position.Compile(Compiler) : Compiler->WorldPosition(WorldPositionOffsetInclude);
	
	if (PosInputIdx == INDEX_NONE)
	{
		return Compiler->Errorf(TEXT("Position compiled error."));
	}

	int32 PosOutputIdx = Compiler->Mul(PosInputIdx,Compiler->Constant(PositionScale));
	
	// 处理SampleOffset输入（如果没有连接则使用常量）
	int32 SampleOffsetInput = SampleOffset.GetTracedInput().Expression ? 
		SampleOffset.Compile(Compiler) : 
		Compiler->Constant(DefaultSampleOffset);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	
	MaterialExpressionCustom->Inputs[0].InputName = TEXT("TextureVolume");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Position") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("SampleOffset") });
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Curl.ush");

	if (SymmetrySample)
	{
		MaterialExpressionCustom->Code = TEXT(R"(
		return CurlNoise3D_Volume(TextureVolume, TextureVolumeSampler, Position, SampleOffset);
		)");
	}
	else
	{
		MaterialExpressionCustom->Code = TEXT(R"(
		return CurlNoise3D_Volume(TextureVolume, TextureVolumeSampler, Position, SampleOffset);
		)");
	}
	
	
	TArray<int32> Inputs{ TextureCodeIndex, PosOutputIdx, SampleOffsetInput };
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionCurlNoise3DVolume::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("CurlNoise3DVolume"));
}

void UMaterialExpressionCurlNoise3DVolume::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Output is a 3D vector field distorted by curl noise."));
	OutToolTip.Add(TEXT("Uses 3D texture sampling to compute the curl of a noise field."));
}

FExpressionInput* UMaterialExpressionCurlNoise3DVolume::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Position;
		case 1: return &VolumeTextureObject;
		case 2: return &SampleOffset;
		default: return nullptr;
	}
	
}

FName UMaterialExpressionCurlNoise3DVolume::GetInputName(int32 InputIndex) const
{
	switch (InputIndex)
	{
		case 0: return TEXT("Position");
		case 1: return TEXT("TextureVolume"); 
		case 2: return TEXT("SampleOffset");
		default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionCurlNoise3DVolume::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0:  // Coordinate
		return MCT_Float3;  // UV 坐标是 Float2
			
	case 1:  // TextureObject - 这就是你要找的！
		return MCT_VolumeTexture;  // ✅ 纹理类型！这就是解决连接问题的关键！
			
	case 2:  // SampleOffset
		return MCT_Float1;  // SampleOffset 是 Float1
			
	default:
		return MCT_Unknown;
	}
}

#endif
