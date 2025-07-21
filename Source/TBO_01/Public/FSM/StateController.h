// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBO_01_API UStateController : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	bool InitializeAtBeginPlay = false;
	/**
		Array of available state classes, this is the easy way to reference UObjects states.\n
		At runtime (in the Init function), the states will be instantiated and the States array will be populated with the pointers to the states instances.\n\n
		The first state in this Array will be the INITIAL STATE.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TSubclassOf<class UStateBase>> AvailableStateClasses;

	/**
	 * This array will contain all the pointers to the state instances created at runtime in the Init function.
	 */
	UPROPERTY()
	TArray<UStateBase*> States;

	/**
	 * Used to handle the tick method of the state controller
	 */
	bool CanTickState = false;

	
public:	
	UStateController();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Function used to change state
	 * @param TargetState The pointer to the target state instance
	 * @return TRUE if change state succeeded, FALSE otherwise
	 */
	UFUNCTION(BlueprintCallable)
	bool ChangeState(UStateBase* TargetState);

	/**
	 * Initialize the state controller and the states
	 */
	UFUNCTION(BlueprintCallable)
	void Init();

	/**
	 * This is useful to retrieve the pointer to the state instance passing the class of the state.\n
	 * Each state is a UObject and cannot have a pointer to another state instance, but only to its class.\n
	 * What this does is pass the States array searching for the first state that is the same type of the StateClass. 
	 * @param StateClass The target state class
	 * @return A pointer to the state instance if available, otherwise a nullptr.
	 */
	UFUNCTION(BlueprintCallable)
	UStateBase* FindStateOfClass(const TSubclassOf<UStateBase> StateClass);

	UFUNCTION(BlueprintCallable)
	void ForceDefaultState();

	/**
	 * The current state
	 */
	UPROPERTY(BlueprintReadOnly)
	UStateBase* CurrentState = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UStateBase* PreviousState = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UStateBase* NextState = nullptr;
};
