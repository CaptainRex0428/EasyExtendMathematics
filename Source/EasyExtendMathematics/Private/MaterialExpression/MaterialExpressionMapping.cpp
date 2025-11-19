#include "MaterialExpression/MaterialExpressionMapping.h"

#include "MaterialCompiler.h"
#include "Materials/MaterialExpressionCustom.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionMapping"

UMaterialExpressionMapping::UMaterialExpressionMapping(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	ConstLocation(0.0f, 0.0f, 0.0f),
	ConstRotation(0.0f, 0.0f, 0.0f),
	ConstScale(1.0f, 1.0f, 1.0f)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_KajiyaKay;
		FConstructorStatics()
			: NAME_KajiyaKay(LOCTEXT("Mapping", "Mapping"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_KajiyaKay);
#endif
}

#if WITH_EDITOR

int32 UMaterialExpressionMapping::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if (!Vector.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing Vector input"));
	}

	// 编译输入
	int32 VectorInput = Vector.Compile(Compiler);
	if (VectorInput == INDEX_NONE)
	{
		return INDEX_NONE;
	}

	// 处理Location输入（如果没有连接则使用常量）
	int32 LocationInput = Location.GetTracedInput().Expression ? 
		Location.Compile(Compiler) : 
		Compiler->Constant3(ConstLocation.X, ConstLocation.Y, ConstLocation.Z);

	// 处理Rotation输入（如果没有连接则使用常量）
	int32 RotationInput = Rotation.GetTracedInput().Expression ? 
		Rotation.Compile(Compiler) : 
		Compiler->Constant3(ConstRotation.X, ConstRotation.Y, ConstRotation.Z);

	// 处理Scale输入（如果没有连接则使用常量）
	int32 ScaleInput = Scale.GetTracedInput().Expression ? 
		Scale.Compile(Compiler) : 
		Compiler->Constant3(ConstScale.X, ConstScale.Y, ConstScale.Z);


	UMaterialExpressionCustom* MaterialExpressionCustom = NewObject<UMaterialExpressionCustom>();
	MaterialExpressionCustom->Inputs[0].InputName = TEXT("Vector");
	MaterialExpressionCustom->Inputs.Add({ TEXT("Location") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("Rotation") });
	MaterialExpressionCustom->Inputs.Add({ TEXT("Scale") });
	MaterialExpressionCustom->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	MaterialExpressionCustom->IncludeFilePaths.Add("/EEShaders/Mapping.ush");

	MaterialExpressionCustom->Code = TEXT(R"(
		float3 rotationRad = radians(Rotation);
		float4x4 mappingMatrix = MTXMapping(Location, rotationRad, Scale);
		return mul(mappingMatrix, float4(Vector, 1.0)).xyz;
		)");
	
	TArray<int32> Inputs{ VectorInput, LocationInput, RotationInput, ScaleInput };
	
	return Compiler->CustomExpression(MaterialExpressionCustom, OutputIndex, Inputs);
}

void UMaterialExpressionMapping::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("Mapping"));
}

void UMaterialExpressionMapping::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	// OutToolTip.Add(TEXT("pow(sqrt(1-pow(dot(t,h),2)),n)"));
	// OutToolTip.Add(TEXT("Output is anisotropy calculated in Kajiyakay."));
	// OutToolTip.Add(TEXT("KajiyaKay include R & TRT."));
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE