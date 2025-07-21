// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStates/PlayerMidAirState.h"
#include "PlayerLedgeFallingState.generated.h"

class UPlayerJumpState;
/**
 * 
 */
UCLASS()
class TBO_01_API UPlayerLedgeFallingState : public UPlayerMidAirState
{
	GENERATED_BODY()

	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void ExitState() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> JumpStateClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Coyote", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float CoyoteTime = 0.2f;

	UPROPERTY()
	UPlayerJumpState* JumpState = nullptr;

	FTimerHandle CoyoteTimeTH;
	bool CanJump = false;

	UFUNCTION()
	void HandleJumpRequest();
	void HandleCoyoteTimeOver();
};
