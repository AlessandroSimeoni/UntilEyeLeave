// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerStateBase.h"
#include "PlayerIdleState.generated.h"

class UPlayerLedgeFallingState;
class UPlayerKnockbackState;
class UPlayerCombatState;
class UPlayerRollState;
class UPlayerJumpState;
class UPlayerStandardMoveState;
/**
 * 
 */
UCLASS()
class TBO_01_API UPlayerIdleState : public UPlayerStateBase
{
	GENERATED_BODY()

public:
	virtual void InitState(UStateController* Controller) override;
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> StandardMoveStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> SlowMoveStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> JumpStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> RollStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> CombatStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> KnockbackStateClass;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Connected States")
	TSubclassOf<UPlayerStateBase> LedgeFallingStateClass;
	
	UPROPERTY()
	UPlayerStandardMoveState* StandardMoveState = nullptr;
	UPROPERTY()
	UPlayerStandardMoveState* SlowMoveState = nullptr;
	UPROPERTY()
	UPlayerJumpState* JumpState = nullptr;
	UPROPERTY()
	UPlayerRollState* RollState = nullptr;
	UPROPERTY()
	UPlayerCombatState* CombatState = nullptr;
	UPROPERTY()
	UPlayerKnockbackState* KnockbackState = nullptr;
	UPROPERTY()
	UPlayerLedgeFallingState* LedgeFallingState = nullptr;

private:
	UFUNCTION()
	void MovementStateTransition(FVector Direction);
	UFUNCTION()
	void GoToJumpState();
	UFUNCTION()
	void GoToRollState();
	UFUNCTION()
	void GoToCombatState();
	UFUNCTION()
	void GoToKnockbackState();
};
