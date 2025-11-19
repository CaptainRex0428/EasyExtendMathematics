#include "MaterialExpression/UVs/MaterialExpressionBumpOffsetThickness.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"
#include "EasyExtendMathematics.h" 
#include "LandscapeRender.h"
#include "Components/DynamicEntryBoxBase.h"

#define LOCTEXT_NAMESPACE "UMatetialExpressionBumpOffsetThickness"

UMaterialExpressionBumpOffsetThickness::UMaterialExpressionBumpOffsetThickness(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// 定义所属分类
		FText Category_Display;
		FConstructorStatics()
			: Category_Display(LOCTEXT("Bump", "Bump"))
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
		DefaultPerlinNoise = Module.GetPerlinNoiseTexture();
	}
	
}

#if WITH_EDITOR

int32 UMaterialExpressionBumpOffsetThickness::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if (!DefaultPerlinNoise)
	{
		return Compiler->Errorf(TEXT("Missing Perlin Noise texture input (defined in node)"));
	}
	
	int32 NormalInput = Normal.GetTracedInput().Expression ? 
		Normal.Compile(Compiler) :
		Compiler->TransformVector(MCB_Tangent,MCB_World,Compiler->Constant3(0,0,1));

	int32 ViewInput = View.GetTracedInput().Expression ? 
		View.Compile(Compiler) : 
		Compiler->CameraVector();

	int32 DepthInput = Depth.GetTracedInput().Expression ? 
		Depth.Compile(Compiler) : 
		Compiler->Constant(DefaultDepth);

	int32 DepthOffsetInput = DepthOffset.GetTracedInput().Expression ? 
		DepthOffset.Compile(Compiler) : 
		Compiler->Mul(Compiler->Constant2(DefaultDepthOffset.X,DefaultDepthOffset.Y),
			Compiler->TextureSample(Compiler->Texture(DefaultPerlinNoise,SAMPLERTYPE_Masks),Compiler->TextureCoordinate(0,0,0),SAMPLERTYPE_Masks));

	int32 ResolutionInput = Resolution.GetTracedInput().Expression ? 
		Resolution.Compile(Compiler) : 
		Compiler->Constant2(DefaultResolution.X,DefaultResolution.Y);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();

	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Normal");
	MaterialExpressionCustom->Inputs.Add({ TEXT("View") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("Depth") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("DepthOffset") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("Resolution") });
	
	
	MaterialExpressionCustom->Code = TEXT(R"(
		return BumpOffsetThickness(Normal, View, Depth, DepthOffset, Resolution);
	)");
	
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float2;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/UVs/Bump.ush");	
	
	TArray<int32> Inputs = TArray<int32>{NormalInput,ViewInput,DepthInput,DepthOffsetInput,ResolutionInput};
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionBumpOffsetThickness::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("BumpOffsetThickness"));
}

void UMaterialExpressionBumpOffsetThickness::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT(""));
}

// this define is only used for the following function
FExpressionInput* UMaterialExpressionBumpOffsetThickness::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Normal;
		case 1: return &View;
		case 2: return &Depth;
		case 3: return &DepthOffset;
		case 4: return &Resolution;
		default: return nullptr;
	}
}

FName UMaterialExpressionBumpOffsetThickness::GetInputName(int32 InputIndex) const
{
	
	switch (InputIndex)
	{
	case 0: return TEXT("Normal");
	case 1: return TEXT("View");
	case 2: return TEXT("Depth");
	case 3: return TEXT("DepthOffset");
	case 4: return TEXT("Resolution");
	default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionBumpOffsetThickness::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0: return MCT_Float3;
	case 1: return MCT_Float3;
	case 2: return MCT_Float1;
	case 3: return MCT_Float1;
	case 4: return MCT_Float2;
	default: return MCT_Unknown;
	}
}

bool UMaterialExpressionBumpOffsetThickness::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

void UMaterialExpressionBumpOffsetThickness::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif
