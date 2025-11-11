#include "MaterialExpression/Sand/MaterialExpressionSandDissolve.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 
#include "LandscapeRender.h"
#include "Components/DynamicEntryBoxBase.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionCurlNoise2D"

UMaterialExpressionSandDissolve::UMaterialExpressionSandDissolve(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	bZDepthMove(false),
DefaultPannerSpeed(0,-1.f),
DefaultSparkingSpeed(1),
DefaultAlpha(0.25),
DefaultRadialScale(1200,5)
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

	int32 ResolutionTextureIdx = Compiler->Constant2(256,256);
	
	int32 PannerSpeedInput = PannerSpeed.GetTracedInput().Expression ? 
		PannerSpeed.Compile(Compiler) : 
		Compiler->Constant2(DefaultPannerSpeed.X,DefaultPannerSpeed.Y);

	int32 SparkingSpeedInput = SparkingSpeed.GetTracedInput().Expression ? 
		SparkingSpeed.Compile(Compiler) : 
		Compiler->Constant(DefaultSparkingSpeed);

	int32 AlphaInput = Alpha.GetTracedInput().Expression ? 
		Alpha.Compile(Compiler) : 
		Compiler->Constant(DefaultAlpha);

	int32 RadialScaleInput = RadialScale.GetTracedInput().Expression ? 
		RadialScale.Compile(Compiler) : 
		Compiler->Constant2(DefaultRadialScale.X,DefaultRadialScale.Y);
	
	// 获取 PixelPosition (屏幕空间像素位置)
	int32 PixelPositionIdx = Compiler->GetPixelPosition();
    
	// 获取 Time (游戏时间)
	int32 GameTimeIdx = Compiler->GameTime(true,200.f);
    
	// 获取 ViewSize (视口尺寸，返回 float2)
	int32 ViewSizeIdx = Compiler->ViewProperty(MEVP_ViewSize);
	int32 ViewCenterIdx = Compiler->Div(ViewSizeIdx, Compiler->Constant(2.f));
	int32 CenterPosInput = bZDepthMove ? (CenterPos.GetTracedInput().Expression ? CenterPos.Compile(Compiler) : ViewCenterIdx ) : ViewCenterIdx;
	
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();

	MaterialExpressionCustom->Inputs[0].InputName = TEXT("NoiseTexture");
	MaterialExpressionCustom->Inputs.Add({ TEXT("resolution") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("UVs") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("time") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("speedMove") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("speedSparking") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("alpha") });
	
	if (bZDepthMove)
	{
		MaterialExpressionCustom->Inputs.Add({ TEXT("centerPos") });
		MaterialExpressionCustom->Inputs.Add({ TEXT("radialScale") });
		
		MaterialExpressionCustom->Code = TEXT(R"(
			return SandDissolveZDepth(
				NoiseTexture, 
				NoiseTextureSampler,
				resolution,
				UVs,
				time,
				speedMove,
				speedSparking,
				alpha,
				centerPos,
				radialScale);
		)");
	}
	else
	{
		MaterialExpressionCustom->Code = TEXT(R"(
			return SandDissolvePlanner(
				NoiseTexture, 
				NoiseTextureSampler,
				resolution,
				UVs,
				time,
				speedMove,
				speedSparking,
				alpha);
		)");
	}
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Dissolve.ush");	
	
	TArray<int32> Inputs = bZDepthMove ?
		TArray<int32>{	TextureIdx,
						ResolutionTextureIdx,
						PixelPositionIdx,
						GameTimeIdx,
						PannerSpeedInput,
						SparkingSpeedInput,
						AlphaInput,
						CenterPosInput,
						RadialScaleInput}
	:	TArray<int32>{	TextureIdx,
						ResolutionTextureIdx,
						PixelPositionIdx,
						GameTimeIdx,
						PannerSpeedInput,
						SparkingSpeedInput,
						AlphaInput};
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
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
		case 0: return &PannerSpeed;
		case 1: return &SparkingSpeed;
		case 2: return &Alpha;
		case 3: return &CenterPos;
		case 4: return &RadialScale;
		default: return nullptr;
		}
	}
	else
	{
		switch (InputIndex)
		{
		case 0: return &PannerSpeed;
		case 1: return &SparkingSpeed;
		case 2: return &Alpha;
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
		case 0: return TEXT("PannerSpeed");
		case 1: return TEXT("SparkingSpeed");
		case 2: return TEXT("Alpha");
		case 3: return TEXT("CenterPos");
		case 4: return TEXT("RadialScale");
		default: return NAME_None;
		}
	}
	else
	{
		switch (InputIndex)
		{
		case 0: return TEXT("PannerSpeed");
		case 1: return TEXT("SparkingSpeed");
		case 2: return TEXT("Alpha");
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
		case 1: return MCT_Float1;
		case 2: return MCT_Float1;
		case 3: return MCT_Float2;
		case 4: return MCT_Float2;
		default: return MCT_Unknown;
		}
	}
	else
	{
		switch (InputIndex)
		{
		case 0: return MCT_Float2;
		case 1: return MCT_Float1;
		case 2: return MCT_Float1;
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
