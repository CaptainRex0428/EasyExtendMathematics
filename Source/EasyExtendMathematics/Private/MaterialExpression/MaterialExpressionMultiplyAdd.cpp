#include "MaterialExpression/MaterialExpressionMultiplyAdd.h"

#include "Materials/MaterialIREmitter.h"
#include "MaterialCompiler.h"

#if WITH_EDITOR
#include "MaterialGraph/MaterialGraphNode_Comment.h"
#include "MaterialGraph/MaterialGraphNode.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif //WITH_EDITOR

#define LOCTEXT_NAMESPACE "UMaterialExpressionMultiplyAdd"

UMaterialExpressionMultiplyAdd::UMaterialExpressionMultiplyAdd(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		FText NAME_MultiplyAdd;
		FConstructorStatics()
			: NAME_MultiplyAdd(LOCTEXT("MultiplyAdd", "MultiplyAdd"))
		{
		}
	};
	
	static FConstructorStatics ConstructorStatics;

	Multiply = 2.f;
	Add = -1.f;
	
#if WITH_EDITORONLY_DATA
		MenuCategories.Add(ConstructorStatics.NAME_MultiplyAdd);
#endif
}

#if WITH_EDITOR

int32 UMaterialExpressionMultiplyAdd::Compile(class FMaterialCompiler* Compiler, int32 OutputIndex)
{
	if(!Input.GetTracedInput().Expression)
	{
		return Compiler->Errorf(TEXT("Missing Value input"));
	}
	
	return Compiler->Add(Compiler->Mul(Input.Compile(Compiler), Compiler->Constant(Multiply)), Compiler->Constant(Add));
}

void UMaterialExpressionMultiplyAdd::GetCaption(TArray<FString>& OutCaptions) const
{
	OutCaptions.Add(TEXT("MultiplyAdd"));
}

void UMaterialExpressionMultiplyAdd::GetExpressionToolTip(TArray<FString>& OutToolTip)
{
	OutToolTip.Add(TEXT("Multiply-Add operation: Value * Multiply + Add"));
	OutToolTip.Add(TEXT("Useful for scaling and offsetting values"));
	OutToolTip.Add(FString::Printf(TEXT("Default Multiply: %.3f"), Multiply));
	OutToolTip.Add(FString::Printf(TEXT("Default Add: %.3f"), Add));
}
#endif
