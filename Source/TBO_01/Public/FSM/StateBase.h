// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.generated.h"

class UStateController;

/**
 * This is the base class for the state
 */
UCLASS(Blueprintable)
class TBO_01_API UStateBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Can the state tick?
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool CanTick = false;
	

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UStateController> StateController;
	UFUNCTION(BlueprintCallable)
	void StateTransition(UStateBase* TargetState) const;
	bool StateReady = true;
	
public:
	/**
	 * Initialize the state.\n
	 * By default, it initializes the StateController field with the one passed as parameter
	 * @param Controller The state controller which this states belong to.
	 */
	virtual void InitState(UStateController* Controller);
	/**
	 * Called when entering the state
	 */
	virtual void EnterState();

	/**
	 * If CanTick = TRUE, this is called every tick by the state controller
	 */
	virtual void TickState(float DeltaTime);

	/**
	 * Called when exiting the state
	 */
	virtual void ExitState();

	bool IsStateReady() const {return StateReady; };

};
