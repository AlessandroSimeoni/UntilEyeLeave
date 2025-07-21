// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LenghtModifier.h"
#include "Interface/SpringArmModifier.h"
#include "UObject/NoExportTypes.h"
#include "DistanceToTargetModifier.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UDistanceToTargetModifier : public UObject, public ISpringArmModifier
{
	GENERATED_BODY()
public:
	static UDistanceToTargetModifier* Create(UObject* Outer, AActor* MainActor, AActor* TargetActor, UCurveFloat* Curve, float DeltaLength, float StartLenght, ULenghtModifier
	                                         * SpringArmLenghtModifier);
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm) override;
	virtual void Revert(bool Immediate) override;
	virtual bool IsDestructible() const override;
	void SetUp(AActor* Actor, UCurveFloat* AlphaCurve, float DeltaArmLength, float StartArmLenght);
	bool IsEnded = false;
	
protected:
	UPROPERTY()
	TObjectPtr<AActor> MainActor;
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;
	UPROPERTY()
	TObjectPtr<UCurveFloat> Curve;
	UPROPERTY()
	TObjectPtr<ULenghtModifier> LenghtModifier;
	
	float DeltaLenght;
	float StartActorDistance;
	float StartLenght;
	float Alpha = 0;
	float ElapsedTime = 0;
	float SmoothSpeed = 10;
	
	
	//** in case is reverted with alpha different to 1 */
	float SmoothRevertDuration = 0.5f;
	float LastAlpha = 0;

	float CalculateDistance() const;
};
