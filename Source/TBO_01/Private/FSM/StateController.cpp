// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/StateController.h"

#include "FSM/StateBase.h"

UStateController::UStateController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateController::BeginPlay()
{
	Super::BeginPlay();

	// initialize states
	if (InitializeAtBeginPlay)
		Init();	
}


// Called every frame
void UStateController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// call TickState of the current state if possible
	if (CanTickState && CurrentState && CurrentState->CanTick)
		CurrentState->TickState(DeltaTime);
}

bool UStateController::ChangeState(UStateBase* TargetState)
{
	if (TargetState)
	{
		if (!TargetState->IsStateReady())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s State is not ready!"), *TargetState->GetName());
			return false;
		}

		NextState = TargetState;
		
		if (CurrentState)
		{
			// cannot change state if the target one is the same as the current one
			if (CurrentState->IsA(TargetState->GetClass()))
			{
				UE_LOG(LogTemp, Error, TEXT("Change State Failed: %s state is equal to current state!"), *TargetState->GetName());
				return false;
			}

			// stop tick and exit the current state
			CanTickState = false;
			CurrentState->ExitState();
		}
		
		PreviousState = CurrentState;

		// enter the target state and re-enable tick
		CurrentState = TargetState;
		CurrentState->EnterState();
		CanTickState = true;

		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Change State Failed: Invalid target state"));
	return false;
}

void UStateController::Init()
{
	// create state objects based on the available player state classes
	for (auto& AvailableState : AvailableStateClasses)
	{
		UStateBase* State = NewObject<UStateBase>(this, AvailableState);
		States.Add(State);
	}

	// initialize states
	for (UStateBase* s : States)
	{
		if (!s)
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid State!"));
			return;
		}

		s->InitState(this);
	}

	// go to initial state (the first one in the array)
	ChangeState(States[0]);
}

UStateBase* UStateController::FindStateOfClass(const TSubclassOf<UStateBase> StateClass)
{
	for (UStateBase* s : States)
	{
		if (s->IsA(StateClass))
			return s;
	}

	return nullptr;
}

void UStateController::ForceDefaultState()
{
	ChangeState(States[0]);
}

