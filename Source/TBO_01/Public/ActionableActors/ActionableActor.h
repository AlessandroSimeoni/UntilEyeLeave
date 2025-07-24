// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActivationOver);

/**
 * This is the base class for an actionable actor.
 * An AActionableActor is an actor with a behaviour that can be triggered by an actuator using the ActivateActor function. 
 */

UCLASS(Abstract, Blueprintable)
class TBO_01_API AActionableActor : public AActor
{
	GENERATED_BODY()

protected:
	AActionableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActionableActor")
	USceneComponent* MyRoot = nullptr;
	
	/**
	 * This is the number of activation requests necessary to activate the behaviour of this actionable actor.
	 * \n(e.g., a door (actionable actor) can be opened only by pulling two levers (actuator actors), so ActivationRequestsRequired = 2)
	 * \n\n Default value: 1 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActionableActor", Meta = (UImin = 1, ClampMin = 1))
	int ActivationRequestsRequired = 1;

	/**
	 * Function containing the logic for activating the behaviour of this actionable actor.
	 * \n Instant activation can be handled by using the input bool parameter.
	 * @param InstantActivation used, if necessary, to handle instant activation of the actor
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateActor(const bool InstantActivation = false);
	virtual void ActivateActor_Implementation(bool InstantActivation = false);

	/**
	 * Function containing the logic for de-activating the behaviour of this actionable actor.
	 * \n Instant de-activation can be handled by using the input bool parameter.
	 * @param InstantDeActivation used, if necessary, to handle instant de-activation of the actor
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeActivateActor(const bool InstantDeActivation = false);
	virtual void DeActivateActor_Implementation(bool InstantDeActivation = false);
	
	int CurrentActivationRequests = 0;
	bool Activated = false;
	
public:
	/**
	 * Requests the activation for this actionable actor
	 * @param InstantActivation Instant activation request
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RequestActivation(const bool InstantActivation);

	/**
	 * Requests the de-activation for this actionable actor
	 * @param InstantDeActivation Instant de-activation request
	 */
	UFUNCTION(BlueprintCallable)
	virtual void RequestDeActivation(const bool InstantDeActivation);
	
	FActivationOver OnActivationOver; 
};
