// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStateBase.h"
#include "PlayerRollState.generated.h"

class UPlayerJumpState;
class UPlayerKnockbackState;
class UPlayerLedgeFallingState;
class UPlayerIdleState;
/**
 * 
 */
UCLASS()
class TBO_01_API UPlayerRollState : public UPlayerStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Roll Animation")
	TObjectPtr<UAnimMontage> RollAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Roll Animation", Meta = (UIMin = 0, UIMax = 2.0f, ClampMin = 0))
	float RollAnimationSpeed = 1.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> IdleStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> FallingLedgeStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> KnockbackStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> JumpStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Roll Cooldown", Meta = (UIMin = 0, UIMax = 5.0f, ClampMin = 0))
	float RollCooldown = 0.5f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Collision Handling", Meta = (UIMin = 0, UIMax = 88.0f, ClampMin = 0))
	float RollCapsuleHalfHeight = 44.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Collision Handling", Meta = (UIMin = 0, UIMax = 100.0f, ClampMin = 0))
	float CharacterMeshOffset = 50.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Collision Handling", Meta = (UIMin = 0, ClampMin = 0))
	float SlideSpeedUnderObjects = 250.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Collision Handling", Meta = (UIMin = 0, ClampMin = 0))
	float UnderObjectEscapeDegrees = 30.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Collision Handling", Meta = (UIMin = 0, ClampMin = 0))
	float SphereCastRadius = 34.0f;;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "States transition animation blending", Meta = (UIMin = 0, ClampMin = 0))
	float ToKnockbackBlendingTime = 0.5f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "States transition animation blending", Meta = (UIMin = 0, ClampMin = 0))
	float ToJumpBlendingTime = 0.1f;

	UPROPERTY(BlueprintReadOnly)
	UPlayerIdleState* IdleState = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UPlayerLedgeFallingState* FallingLedgeState = nullptr;
	UPROPERTY()
	UPlayerKnockbackState* KnockbackState = nullptr;
	UPROPERTY()
	UPlayerJumpState* JumpState = nullptr;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Roll Function")
	void StartRolling();
	UFUNCTION(BlueprintCallable, Category = "Roll Function")
	bool CheckObstacleAboveHead();
	UFUNCTION(BlueprintCallable, Category = "Roll Function")
	void TryEndRolling();
	
	FTimerHandle RollCooldownTimer;
	float DefaultCharacterCapsuleHalfHeight = 88.0f;
	FVector SphereCastStart;
	FVector SphereCastEnd;

	FCollisionQueryParams CollisionParams;
	FHitResult HitResult;
	bool IsUnderObject = false;
	bool FrontWallObstacle = false;
	float FrontWallSphereDetectionLength = 40.0f;
	float FrontWallSafeOffset = 30.0f;
	float FrontWallSphereDetectionRadius = 20.0f;
	FVector FrontWallCrossResult = FVector::ZeroVector;
	
	UFUNCTION()
	void EnableRoll();
	UFUNCTION()
	void GoToKnockbackState();
	UFUNCTION()
	void GoToJumpState();
	
public:
	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;
};
