// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStateBase.h"
#include "PlayerMidAirState.generated.h"

class UPlayerRollState;
class UPlayerKnockbackState;
class UPlayerCombatState;
class UPlayerLedgeGrabState;
class UPlayerIdleState;

UCLASS()
class TBO_01_API UPlayerMidAirState : public UPlayerStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> IdleStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> LedgeGrabStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> CombatStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> KnockbackStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> RollStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "To Roll State Settings", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float RollInputTimeout = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	USoundBase* BeginMidAirSFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	USoundBase* EndMidAirSFX = nullptr;
	
	/**
	 * Gravity scale while falling/jumping
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Gravity Scale",  Meta = (UIMin = 0.0f, UIMax = 10.0f, ClampMin = 0.0f, ClampMax = 10.0f))
	float FallingGravityScale = 1.0f;
	/**
	 * Movement control factor along the FORWARD direction of the character.\n
	 * 0 -> no control\n
	 * >0 -> more control
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mid Air Control",  Meta = (UIMin = 0.0f, UIMax = 3.0f, ClampMin = 0.0f, ClampMax = 3.0f))
	float ForwardControlFactor = 0.5f;
	/**
	 * Movement control factor along the HORIZONTAL direction of the character.\n
	 * 0 -> no control\n
	 * >0 -> more control
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mid Air Control",  Meta = (UIMin = 0.0f, UIMax = 3.0f, ClampMin = 0.0f, ClampMax = 3.0f))
	float HorizontalControlFactor = 0.5f;
	/**
	 * Movement speed while in air [cm/s]
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mid Air Control",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MidAirMaxSpeed = 600.0f;

	/**
	 * If FALSE (by default), ledge grab will work always while in midair.\n
	 * If TRUE, ledge grab will work only when the character is falling (falling after a jump or from a ledge).
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	bool FallingOnlyLedgeGrab = false;
	
	/**
	 * Vertical offset from the player position for the VERTICAL trace of detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	float VerticalRayUpwardOffset = 88.0f;

	/**
	 * Forward offset from the player position for the VERTICAL trace of detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	float VerticalRayForwardOffset = 50.0f;

	/**
	 * Length of the VERTICAL trace for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float VerticalRayLength = 50.0f;

	/**
	 * Radius of the VERTICAL trace for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float VerticalRayRadius = 5.0f;

	/**
	 * Length of the HORIZONTAL trace for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float HorizontalRayLength = 100.0f;

	/**
	 * Radius of the HORIZONTAL trace for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float HorizontalRayRadius = 5.0f;

	/**
	 * Length of the HORIZONTAL trace for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float PelvisFromLedgeMaxOffset = 50.0f;
	
	UPROPERTY()
	UPlayerIdleState* IdleState = nullptr;
	UPROPERTY()
	UPlayerLedgeGrabState* LedgeGrabState = nullptr;
	UPROPERTY()
	UPlayerCombatState* CombatState = nullptr;
	UPROPERTY()
	UPlayerKnockbackState* KnockbackState = nullptr;
	UPROPERTY()
	UPlayerRollState* RollState = nullptr;
		
	FTimerHandle RollInputTH;
	bool RollRequested = false;
	FVector ForwardMovementComponent;
	FVector HorizontalMovementComponent;
	float PreviousMaxWalkSpeed;

	UFUNCTION()
	virtual void HandleMidAirMovement(FVector MovementDirection);
	UFUNCTION()
	virtual void HandleMidAirAttack();
	UFUNCTION()
	virtual void GoToKnockbackState();
	UFUNCTION()
	void HandleRollRequest();
	void CancelRollRequest();

public:
	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;
	
};
