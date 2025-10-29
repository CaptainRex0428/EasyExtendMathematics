// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyExtendMathematicsBPLibrary.h"
#include "EasyExtendMathematics.h"
#include "EasyMath.h"

UEasyExtendMathematicsBPLibrary::UEasyExtendMathematicsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UEasyExtendMathematicsBPLibrary::EasyExtendMathematicsSampleFunction(float Param)
{
	return -1;
}


FVector UEasyExtendMathematicsBPLibrary::EasyMathTranslateFunction(AActor* actor , FVector VecIn, FVector Source)
{
	if (!actor)
	{
		return FVector::ZeroVector; // 安全检查
	}
        
	auto matrix = EM::MTXTranslation(VecIn[0],VecIn[1],VecIn[2]);
	
	EM::Vector<double,4> o = EM::Vector<double,4>{Source[0], Source[1], Source[2], 1};

	EM::Matrix<double,4, 1> m = matrix * o;
	FVector NewLocation(m[0], m[1], m[2]);
	actor->SetActorLocation(NewLocation);

	return NewLocation;
}

FVector UEasyExtendMathematicsBPLibrary::EasyMathRotateFunction(AActor* actor, FVector VecIn, FVector Source)
{
	if (!actor)
	{
		return FVector::ZeroVector; // 安全检查
	}

	auto mx = EM::MTXRotationX(VecIn[0] * PI / 180);
	auto my = EM::MTXRotationY(VecIn[1] * PI / 180);
	auto mz = EM::MTXRotationZ(VecIn[2] * PI / 180);

	auto mt = EM::MTXTranslation(Source[0],Source[1],Source[2]);
	auto mt_inv = EM::MTXTranslation(-Source[0],-Source[1],-Source[2]);

	auto m = mt * mx * my * mz * mt_inv;

	FVector CurrentLocation = actor->GetActorLocation();
	EM::Vector<double,4> pos = EM::Vector<double,4>{CurrentLocation[0], CurrentLocation[1], CurrentLocation[2], 1};
	
	EM::Matrix<double,4, 1> location = m * pos;
	
	FVector NewLocation(location[0], location[1], location[2]);
	
	actor->SetActorLocation(NewLocation);
	
	return NewLocation;
}

FVector UEasyExtendMathematicsBPLibrary::EasyMathScaleFunction(AActor* actor, FVector VecIn, FVector Source)
{
	if (!actor)
	{
		return FVector::ZeroVector; // 安全检查
	}

	auto matrix = EM::MTXScale(VecIn[0], VecIn[1], VecIn[2]);
	EM::Vector<double,4> o = EM::Vector<double,4>{Source[0], Source[1], Source[2], 1};

	EM::Matrix<double,4, 1> m = matrix * o;
	FVector NewScale(m[0], m[1], m[2]);
	actor->SetActorScale3D(NewScale);

	return NewScale;
}
