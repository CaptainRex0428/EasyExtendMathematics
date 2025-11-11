#include "MaterialExpression/Tangent/MaterialExpressionTangentWS.h"

#include "MaterialCompiler.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionTangentWS"

UMaterialExpressionTangentWS::UMaterialExpressionTangentWS(const FObjectInitializer& ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_TangentWS;
		FConstructorStatics()
			: NAME_TangentWS(LOCTEXT("TangentWS", "TangentWS"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_TangentWS);
#endif
}

int32 UMaterialExpressionTangentWS::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if(!NormalWS.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing NormalWS input"));
	}

	if(!TangentRotate.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing TangentRotate input"));
	}

	int32 normalInput = NormalWS.Compile(Compiler);
	int32 TangentRotateInput  = TangentRotate.Compile(Compiler);

	int32 Binormal = Compiler->Normalize(Compiler->Lerp(Compiler->Constant3(1,0,1),Compiler->Constant3(0,1,0),TangentRotateInput));
	
	return Obverse ?  Compiler->Cross(Binormal,normalInput) : Compiler->Cross(normalInput, Binormal);
}

void UMaterialExpressionTangentWS::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("TangentWS"));
}

void UMaterialExpressionTangentWS::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("TBN calculate in world space."));
	OutToolTip.Add(TEXT("Output is in world space."));
}
