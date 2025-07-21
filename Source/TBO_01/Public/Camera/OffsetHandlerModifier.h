// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockOnModifier.h"
#include "OffsetToForwardModifier.h"
#include "Interface/SpringArmModifier.h"
#include "OffsetHandlerModifier.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOffsetUpdate, FVector, NewOffset);
UCLASS()
class TBO_01_API UOffsetHandlerModifier : public UObject,public ISpringArmModifier
{
public:
	GENERATED_BODY()
	static UOffsetHandlerModifier* Create(UObject* Outer,AActor* MainActor,float LockOffset,float ForwardOffset,float InterpSpeed,float MinDistance,float MaxDistance);
	virtual void Update(float DeltaTime, USpringArmComponent* SpringArm) override;
	virtual void Revert(bool Immediate) override;
	virtual bool IsDestructible() const override;
	void SetStatus(bool Status,bool SetToZero);
	void SetDefaultOffset(bool UseDefaultOffset,const FVector& DefaultOffsetValue){IsDefaultOffset = UseDefaultOffset, DefaultOffset = DefaultOffsetValue;}
	
	UPROPERTY()
	TObjectPtr<UOffsetToForwardModifier> OffsetForwardModifier;
	UPROPERTY()
	TObjectPtr<ULockOnModifier> LockOnModifier;

	
	bool Enabled = true;
	bool ImmediateStatus = false;
	bool IsDefaultOffset = false;
	FOnOffsetUpdate OnOffsetUpdate;
protected:
	float InterpSpeed;
	FVector DefaultOffset = FVector::ZeroVector;
	
	
};
