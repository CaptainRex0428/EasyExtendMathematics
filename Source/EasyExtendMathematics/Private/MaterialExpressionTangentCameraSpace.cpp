#include "MaterialExpressionTangentCameraSpace.h"

#include "MaterialCompiler.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionTangentCameraSpace"

UMaterialExpressionTangentCameraSpace::UMaterialExpressionTangentCameraSpace(const FObjectInitializer& ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_TangentCameraSpace;
		FConstructorStatics()
			: NAME_TangentCameraSpace(LOCTEXT("TangentCameraSpace", "TangentCameraSpace"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_TangentCameraSpace);
#endif
}

int32 UMaterialExpressionTangentCameraSpace::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if(!NormalWS.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing NormalWS input"));
	}

	if(!TangentRotate.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing TangentRotate input"));
	}

	int32 normalInput = Compiler->TransformVector(MCB_World,MCB_Camera,NormalWS.Compile(Compiler));
	int32 TangentRotateInput  = TangentRotate.Compile(Compiler);

	int32 Binormal = Compiler->Normalize(Compiler->Lerp(Compiler->Constant3(1,0,1),Compiler->Constant3(0,1,0),TangentRotateInput));
	
	int32 tangentCS = Obverse ?  Compiler->Cross(Binormal,normalInput) : Compiler->Cross(normalInput, Binormal);

	return Compiler->TransformVector(MCB_Camera,MCB_World,tangentCS);
}

void UMaterialExpressionTangentCameraSpace::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("TangentCameraSpace"));
}

void UMaterialExpressionTangentCameraSpace::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("TBN calculate in camera space."));
	OutToolTip.Add(TEXT("Output is in world space."));
}
