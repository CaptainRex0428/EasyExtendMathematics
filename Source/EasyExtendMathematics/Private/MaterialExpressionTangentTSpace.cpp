#include "MaterialExpressionTangentTSpace.h"

#include "MaterialCompiler.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionTangent"

UMaterialExpressionTangentTSpace::UMaterialExpressionTangentTSpace(const FObjectInitializer& ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_Tangent;
		FConstructorStatics()
			: NAME_Tangent(LOCTEXT("Tangent", "Tangent"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_Tangent);
#endif
}

int32 UMaterialExpressionTangentTSpace::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if(!NormalTangent.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing NormalTangent input"));
	}

	if(!TangentRotate.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing TangentRotate input"));
	}

	int32 normalInput = NormalTangent.Compile(Compiler);
	int32 TangentRotateInput  = TangentRotate.Compile(Compiler);

	int32 Binormal = Compiler->Normalize(Compiler->Lerp(Compiler->Constant3(1,0,1),Compiler->Constant3(0,1,0),TangentRotateInput));
	
	return Obverse ?  Compiler->Cross(Binormal,normalInput) : Compiler->Cross(normalInput, Binormal);
}

void UMaterialExpressionTangentTSpace::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("Tangent"));
}

void UMaterialExpressionTangentTSpace::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("TBN calculate in tangent space."));
	OutToolTip.Add(TEXT("Output is in tangent space."));
}
