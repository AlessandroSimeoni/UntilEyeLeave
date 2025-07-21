// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SpringArmModifier.h"
#include "UObject/NoExportTypes.h"
#include "LockOnModifier.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API ULockOnModifier : public UObject, public ISpringArmModifier
{
	GENERATED_BODY()
public:
	static ULockOnModifier* Create(UObject* Outer,AActor* MainActor,AActor* TargetActor,float Offset, float InterpSpeed, float MinDistance,float MaxDistance);
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm) override;
	virtual void Revert(bool Immediate) override;
	virtual bool IsDestructible() const override;
	
	
	/** set min and max as the squared parameter  */
	void SetMinMaxDistance(float Min, float Max);

	/** Getter for TargetActor */
	AActor* GetTargetActor();
	
	/** Setter for Target Actor */
	void SetTargetActor(AActor* NewTarget);

	/** check null targetActor */
	bool TargetIsNull() const;

	float Offset;
	float InterpSpeed;
	float MinDistance = 100.0f; 
	float MaxDistance = 1000.0f;
	

protected:
	UPROPERTY()
	TObjectPtr<AActor> MainActor;
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	
};
