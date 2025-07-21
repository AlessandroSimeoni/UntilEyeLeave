// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStates/PlayerMidAirState.h"
#include "PlayerJumpState.generated.h"

class UPlayerRollState;

UCLASS()
class TBO_01_API UPlayerJumpState : public UPlayerMidAirState
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Jump Height",  Meta = (Units = "Centimeters", UIMin = 0.0f, ClampMin = 0.0f))
	float JumpHeight = 100.0f;
	/**
	 * Movement control factor along the FORWARD direction of the character.\n
	 * 0 -> no control\n
	 * >0 -> more control
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "From Idle Mid Air Control",  Meta = (UIMin = 0.0f, UIMax = 3.0f, ClampMin = 0.0f, ClampMax = 3.0f))
	float FromIdleForwardControlFactor = 0.0f;
	/**
	 * Movement control factor along the HORIZONTAL direction of the character.\n
	 * 0 -> no control\n
	 * >0 -> more control
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "From Idle Mid Air Control",  Meta = (UIMin = 0.0f, UIMax = 3.0f, ClampMin = 0.0f, ClampMax = 3.0f))
	float FromIdleHorizontalControlFactor = 0.0f;
	
	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void ExitState() override;

	virtual void HandleMidAirMovement(FVector MovementDirection) override;
	virtual void HandleMidAirAttack() override;

private:
	float TargetForwardControl = 0.0f;
	float TargetHorizontalControl = 0.0f;
};
