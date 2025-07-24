// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GameplayObjectState.h"
#include "ActuatorActor.generated.h"

class USaveSubsystem;
class UUniqueIDComponent;
class AActionableActor;

/**
 * This is the base class for an actuator actor.
 * An AActuatorActor is an actor that can trigger one or more AActionableActor connected to it. 
 */


UCLASS()
class TBO_01_API AActuatorActor : public AActor, public IGameplayObjectState
{
	GENERATED_BODY()

protected:
	AActuatorActor();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActionableActor")
	USceneComponent* MyRoot = nullptr;

	/**
	 * Each AActuatorActor has a UUniqueIDComponent used to save the state of the actuator itself and the actionables connected to it
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorComponent")
	UUniqueIDComponent* UniqueIDComp = nullptr;


	/**
	 * The actionable actors connected to this actuator
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connected Actionable Actors")
	TArray<TObjectPtr<AActionableActor>> ConnectedActionableActor;
	/**
	 * Implementation of the RestoreState function of the IGameplayObjectState.
	 * \n It is used to handle and restore the state of the actor when it is saved
	 */
	virtual void RestoreState_Implementation() override;

	/**
	 * Implementation of the HasBeenActivated function of the IGameplayObjectState
	 * @return Activated boolean
	 */
	virtual bool HasBeenActivated_Implementation() override;

	UPROPERTY()
	USaveSubsystem* SaveSubsystem = nullptr;

	bool Activated = false;
	
public:
	/**
	 * Request the activation to all the actionable actors connected to this actuator
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ActionActors(const bool InstantActivation = false);

	/**
	 * Request the de-activation to all the actionable actors connected to this actuator
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DeActivateActors(const bool InstantDeActivation = false);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRestore();
};
