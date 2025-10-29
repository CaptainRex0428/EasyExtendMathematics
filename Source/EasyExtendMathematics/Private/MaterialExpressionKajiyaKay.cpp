#include "MaterialExpressionKajiyakay.h"

#include "MaterialCompiler.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionKajiyaKay"

UMaterialExpressionKajiyaKay::UMaterialExpressionKajiyaKay(const FObjectInitializer& ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_KajiyaKay;
		FConstructorStatics()
			: NAME_KajiyaKay(LOCTEXT("KajiyaKay", "KajiyaKay"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;
	
#if WITH_EDITORONLY_DATA
	MenuCategories.Add(ConstructorStatics.NAME_KajiyaKay);
#endif
}

int32 UMaterialExpressionKajiyaKay::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if(!Tangent.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing Tangent input"));
	}

	if(!LightDirection.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing LightDirection input"));
	}

	if(!ViewDirection.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing ViewDirection input"));
	}

	if(!ViewDirection.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing Exponent input"));
	}

	int32 ViewDirectionResult =  UseHalfVector ? Compiler->Normalize(Compiler->Add( ViewDirection.Compile(Compiler),LightDirection.Compile(Compiler))) : ViewDirection.Compile(Compiler); 
	int32 t = Tangent.Compile(Compiler);
	int32 e = Exponent.Compile(Compiler);

	int32 dotth = Compiler->Dot(ViewDirectionResult,t);
	int32 smoothMask = Compiler->SmoothStep(-1,0,dotth);
	int32 sinTH = Compiler->Power(Compiler->SquareRoot(Compiler->Sub(Compiler->Constant(1),Compiler->Dot(dotth,dotth))),e);
	
	return Compiler->Mul( smoothMask, sinTH);
}

void UMaterialExpressionKajiyaKay::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("KajiyaKay"));
}

void UMaterialExpressionKajiyaKay::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("pow(sqrt(1-pow(dot(t,h),2)),n)"));
	OutToolTip.Add(TEXT("Output is anisotropy calculated in Kajiyakay."));
	OutToolTip.Add(TEXT("KajiyaKay include R & TRT."));
}
