// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStateBase.h"
#include "PlayerLedgeGrabState.generated.h"

class UCapsuleComponent;
class UPlayerIdleState;
/**
 * 
 */
UCLASS()
class TBO_01_API UPlayerLedgeGrabState : public UPlayerStateBase
{
	GENERATED_BODY()

	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void ExitState() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> IdleStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Mid Air|On Grab", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float CapsuleDistanceFromWall = 10.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Mid Air|On Grab", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float CapsuleVerticalOffsetFromLedge = 88.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Mid Air|On End", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MidAirEndingCapsuleForwardOffset = 50.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Mid Air|Animations Speed", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float MidAirClimbPlayRate = 2.5f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Grounded|On End", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float GroundedEndingCapsuleForwardOffset = 50.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Grounded|On End", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float GroundedEndingCapsuleUpwardOffset = 50.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Grounded|Animations Speed", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float GroundedClimbPlayRate = 2.0f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ledge Grab Options|Grounded|Animations Speed", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float GroundCheckBeforeClimbDistance = 100.0f;

	UPROPERTY()
	UPlayerIdleState* IdleState = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Ledge Grab Function")
	void EndLedgeGrab();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInit();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ledge Grab Function")
	void StartMidAirLedgeGrab();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ledge Grab Function")
	void StartGroundedLedgeGrab();
public:
	UPROPERTY(BlueprintReadOnly)
	FHitResult HorizontalLedgeHitResult;
	UPROPERTY(BlueprintReadOnly)
	FHitResult VerticalLedgeHitResult;
	UFUNCTION()
	bool CheckTargetGroundNearby() const;
};
