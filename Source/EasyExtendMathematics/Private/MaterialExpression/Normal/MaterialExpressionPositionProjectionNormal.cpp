#include "MaterialExpression/Normal/MaterialExpressionPositionProjectionNormal.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionPositionProjectionNormal"

UMaterialExpressionPositionProjectionNormal::UMaterialExpressionPositionProjectionNormal(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	DefaultThreshold(1.f,1.f,1.f),
	DefaultOffset(0.f,0.f,0.f)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// 定义所属分类
		FText Category_Display;
		FConstructorStatics()
			: Category_Display(LOCTEXT("Normal", "Normal"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.Category_Display);
#endif
	
}

#if WITH_EDITOR

int32 UMaterialExpressionPositionProjectionNormal::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{

	int32 PositionInput = Position.GetTracedInput().Expression ? 
			Position.Compile(Compiler) :
			Compiler->LocalPosition(EPositionIncludedOffsets::IncludeOffsets, ELocalPositionOrigin::Instance);
	
	int32 ThresholdInput = Threshold.GetTracedInput().Expression ? 
		Threshold.Compile(Compiler) :
		Compiler->Constant3(DefaultThreshold.X, DefaultThreshold.Y, DefaultThreshold.Z);

	int32 OffsetInput = Offset.GetTracedInput().Expression ? 
		Offset.Compile(Compiler) : 
		Compiler->Constant3(DefaultOffset.X, DefaultOffset.Y, DefaultOffset.Z);
	
	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	
	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Position");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Threshold") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("Offset") });
	
	
	MaterialExpressionCustom->Code = TEXT(R"(
		return VectorRemap(Position, Threshold, Offset);
	)");
	
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Mapping.ush");	
	
	TArray<int32> Inputs = TArray<int32>{PositionInput, ThresholdInput, OffsetInput};
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionPositionProjectionNormal::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("PositionProjectionNormal"));
}

void UMaterialExpressionPositionProjectionNormal::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT(""));
}

// this define is only used for the following function
FExpressionInput* UMaterialExpressionPositionProjectionNormal::GetInput(int32 InputIndex)
{
	switch (InputIndex)
	{
		case 0: return &Position;
		case 1: return &Threshold;
		case 2: return &Offset;
		default: return nullptr;
	}
}

FName UMaterialExpressionPositionProjectionNormal::GetInputName(int32 InputIndex) const
{
	
	switch (InputIndex)
	{
	case 0: return TEXT("Position");
	case 1: return TEXT("Threshold");
	case 2: return TEXT("Offset");
	default: return NAME_None;
	}
}

EMaterialValueType UMaterialExpressionPositionProjectionNormal::GetInputValueType(int32 InputIndex)
{
	switch (InputIndex)
	{
	case 0: return MCT_Float3;
	case 1: return MCT_Float3;
	case 2: return MCT_Float3;
	default: return MCT_Unknown;
	}
}

bool UMaterialExpressionPositionProjectionNormal::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

void UMaterialExpressionPositionProjectionNormal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif
