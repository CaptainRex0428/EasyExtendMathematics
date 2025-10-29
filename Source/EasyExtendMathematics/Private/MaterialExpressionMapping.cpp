#include "MaterialExpressionMapping.h"

#include "MaterialCompiler.h"

#define LOCTEXT_NAMESPACE "UMaterialExpressionMapping"

UMaterialExpressionMapping::UMaterialExpressionMapping(const FObjectInitializer& ObjectInitializer)
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

int32 UMaterialExpressionMapping::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	return Compiler->Floor(1);
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
