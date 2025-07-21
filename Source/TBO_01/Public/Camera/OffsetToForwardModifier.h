// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SpringArmModifier.h"
#include "OffsetToForwardModifier.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UOffsetToForwardModifier : public UObject, public ISpringArmModifier
{
	GENERATED_BODY()

public:

	static UOffsetToForwardModifier* Create(UObject* Outer,AActor* Target, float OffsetValue,float InterpSpeed);
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm) override;
	virtual void Revert(bool Immediate) override;
	virtual bool IsDestructible() const override;
	void  SetParameter(AActor* InTarget, float InOffsetValue, float InInterpSpeed);
	

	/** Setter for Target Actor */
	void SetTargetActor(AActor* NewTarget);

	/** check null targetActor */
	bool TargetIsNull() const ;

	//** Set Offset */
	void SetOffset(float NewOffset);
	
protected:
	UPROPERTY()
	TObjectPtr<AActor> Target;
	float StartOffsetValue;
	float Offset;
	float InterpSpeed;
	bool IsReverting = false;

	// Raycast Parameter
	const float SphereRadius = 30.f;     
	const float RayDistance = 100.f;     
	const float CollisionAvoidanceOffset = 100.f; 
	const float ForwardOffset = 40.f;
	FHitResult LeftHit, RightHit;


	
};
