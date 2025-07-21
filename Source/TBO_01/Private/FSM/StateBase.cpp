// Fill out your copyright notice in the Description page of Project Settings.


#include "FSM/StateBase.h"

#include "FSM/StateController.h"

void UStateBase::InitState(UStateController* Controller)
{
	StateController = Controller;
}

void UStateBase::EnterState()
{
}

void UStateBase::TickState(float DeltaTime)
{
	if (!CanTick)
		return;
}

void UStateBase::ExitState()
{
}


void UStateBase::StateTransition(UStateBase* TargetState) const
{
	if (TargetState)
		StateController->ChangeState(TargetState);
}
