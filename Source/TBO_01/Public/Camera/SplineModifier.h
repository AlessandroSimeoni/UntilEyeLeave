// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LenghtModifier.h"
#include "RotationModifier.h"
#include "Components/SplineComponent.h"
#include "Interface/SpringArmModifier.h"
#include "SplineModifier.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API USplineModifier : public UObject, public ISpringArmModifier
{
	GENERATED_BODY()

public:
	static USplineModifier* Create(UObject* Outer, USplineComponent* Spline, URotationModifier* RotationModifier, ULenghtModifier* LengthModifier);
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm)override; 
	virtual void Revert(bool Immediate)override;
	virtual bool IsDestructible() const override;
	void SetUp(USplineComponent* Spline, const FVector& Start, const FVector& End, bool UseStartEndPointDistance, bool
	           AdaptSpringArmLenght, bool Dolly, bool DollyAligned, bool ForwardAligned, float RotationSpeed, float LenghtSpeed, float
	           SplineSpeed);
	bool IsEnded = false;
	
protected:
	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent;
	UPROPERTY()
	TObjectPtr<URotationModifier> RotationModifier;
	UPROPERTY()
	TObjectPtr<ULenghtModifier> LenghtModifier;
	FVector EndPoint;

	float SpringArmRotationSpeed = 1.0f;
	float SpringArmLenghtSpeed = 1.0f;
	float SpeedAlongSpline = 3.0f;
	float TargetDistanceAlongSpline;
	float CurrentDistance = 0;
	float CurrentDistanceAlongSpline = 0;
	float StartToEndDistance = 0;
	
	bool IsUsingStartEndPoint = false;
	bool IsAdaptingSpringArmLength = false;
	bool IsDolly = false;
	bool IsDollyAligned = false;
	bool IsForwardAligned = false;
	
};
