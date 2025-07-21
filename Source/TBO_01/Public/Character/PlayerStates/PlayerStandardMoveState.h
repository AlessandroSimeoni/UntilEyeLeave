// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStateBase.h"
#include "PlayerStandardMoveState.generated.h"

class UPlayerKnockbackState;
class UPlayerCombatState;
class UPlayerLedgeGrabState;
class UPlayerRollState;
class UPlayerJumpState;
class UPlayerLedgeFallingState;
class UCharacterMovementComponent;
class UPlayerIdleState;
/**
 * 
 */
UCLASS()
class TBO_01_API UPlayerStandardMoveState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;
	FVector MovementDirection = FVector::ZeroVector;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement State", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MaxWalkSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement State")
	bool IsSlowMovementState = false;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> IdleStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> LedgeFallingStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> JumpStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> OtherMoveModeStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> RollStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> LedgeGrabStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> CombatStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> KnockbackStateClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	bool CanLedgeGrab = false;

	/**
	 * Threshold to activate ledge grab when near the target ground to climb.\n
	 * A higher value means that the movement direction must be towards the target ground to activate the ledge grab.\n
	 * Useful to avoid climbing when near the ground, but you want to move in a different direction.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection", Meta = (UIMin = -1.0f, UIMax = 1.0f, ClampMin = -1.0f, ClampMax = 1.0f))
	float WallNormalToMoveDirectionDotThreshold = false;
	
	/**
	 * Vertical offset from the player position for the VERTICAL ray of detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	float VerticalRayUpwardOffset = 88.0f;

	/**
	 * Forward offset from the player position for the VERTICAL ray of detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	float VerticalRayForwardOffset = 50.0f;

	/**
	 * Length of the VERTICAL ray for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float VerticalRayLength = 50.0f;

	/**
	 * Radius of the VERTICAL ray for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float VerticalRayRadius = 5.0f;

	/**
	 * Vertical offset from the player position for the HORIZONTAL ray of detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection")
	float HorizontalRayUpwardOffset = 0.0f;
	
	/**
	 * Length of the HORIZONTAL ray for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float HorizontalRayLength = 100.0f;

	/**
	 * Radius of the HORIZONTAL ray for the ledge detection
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float HorizontalRayRadius = 5.0f;

	/**
	 * The max height of the step the player can climb
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Detection",  Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MaxStepHeight = 88.0f;

	float PelvisFromLedgeMaxOffset = 9999.0f;
	
	UPROPERTY()
	UPlayerIdleState* IdleState = nullptr;
	UPROPERTY()
	UPlayerLedgeFallingState* LedgeFallingState = nullptr;
	UPROPERTY()
	UPlayerJumpState* JumpState = nullptr;
	UPROPERTY()
	UPlayerStandardMoveState* OtherMoveModeState = nullptr;
	UPROPERTY()
	UPlayerRollState* RollState = nullptr;
	UPROPERTY()
	UPlayerLedgeGrabState* LedgeGrabState = nullptr;
	UPROPERTY()
	UPlayerCombatState* CombatState = nullptr;
	UPROPERTY()
	UPlayerKnockbackState* KnockbackState = nullptr;
	
	/**
	 * Threshold value to detect an abrupt direction change.\n
	 * Clamped between -1 and 1.\n
	 * A great value correspond to a more sensitive detection. 
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abrupt Direction Change", Meta = (UIMin = -1.0f, UIMax = 1.0f, ClampMin = -1.0f, ClampMax = 1.0f))
	float AbruptDirectionDetectionThreshold = 0.0f;
	
	/**
	 * Wait time after an abrupt change in direction.\n
	 * Player will not move for this amount of seconds.\n
	 * Put this equal to 0 to DISABLE the abrupt direction detection system.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abrupt Direction Change", Meta = (UIMin = 0.0f, UIMax = 0.5f, ClampMin = 0.0f, ClampMax = 0.5f))
	float AbruptDirectionWaitTime = 0.1f;
	
	/**
	 * When an abrupt change in direction occur, player will rotate towards the target direction at this rate. 
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abrupt Direction Change", Meta = (UIMin = 0.0f, UIMax = 3000.0f, ClampMin = 0.0f))
	float AbruptDirectionYawRotationRate = 1500.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Abrupt Direction Change")
	FTimerHandle AbruptDirectionTimer;
	
	float SquaredSpeed = 0.0f;
	FVector LastMovementDirection = FVector::ZeroVector;
	FTimerManager* TM; 
	
	void ConsumeMovement();
	UFUNCTION()
	void AddMovementDirection(FVector Direction);
	UFUNCTION()
	void RestartMovingAfterWait() const;
	UFUNCTION()
	void GoToJumpState();
	UFUNCTION()
	void GoToRollState();
	UFUNCTION()
	void GoToCombatState();
	UFUNCTION()
	void GoToKnockbackState();

	void SetCharacterMaxWalkSpeed() const;
	void ResetCharacterRotationRate() const;
};
