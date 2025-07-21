// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion|Idle to Run", Meta = (UIMin = 0.0f, UIMax = 1.0f, ClampMin = 0.0f, ClampMax = 1.0f))
	float IdleToRunRatio = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion|Idle to Run", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float IdleToRunLerpSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool IsJumping = false;
	
private:
	UPROPERTY()
	APawn * OwnerPawn;
	UPROPERTY()
	class AMainCharacter * OwnerCharacter;
	float TargetIdleToRunValue = 0.0f;
	bool IdleToRunLerp = false;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void StartIdleToRunInterpolation(float TargetValue);
};
