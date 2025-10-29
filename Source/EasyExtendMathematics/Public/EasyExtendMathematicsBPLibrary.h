// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"

#include "EasyExtendMathematicsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UEasyExtendMathematicsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "EasyExtendMathematics sample test testing"), Category = "EasyExtendMathematicsTesting")
	static float EasyExtendMathematicsSampleFunction(float Param);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Easy Math Translate", Keywords = "EasyMath Translate"), Category = "EasyMath|Translate")
	static FVector EasyMathTranslateFunction(AActor* actor , FVector VecIn, FVector Source = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Easy Math Rotate", Keywords = "EasyMath Translate"), Category = "EasyMath|Rotate")
	static FVector EasyMathRotateFunction(AActor* actor , FVector VecIn, FVector Source = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Easy Math Scale", Keywords = "EasyMath Translate"), Category = "EasyMath|Rotate")
	static FVector EasyMathScaleFunction(AActor* actor , FVector VecIn, FVector Source = FVector(1,1,1));
};
