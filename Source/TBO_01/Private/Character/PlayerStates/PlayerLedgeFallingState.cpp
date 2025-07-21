// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerLedgeFallingState.h"

#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerJumpState.h"
#include "Character/PlayerStates/PlayerStandardMoveState.h"
#include "FSM/StateController.h"

void UPlayerLedgeFallingState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	JumpState = Cast<UPlayerJumpState>(StateController->FindStateOfClass(JumpStateClass));
}

void UPlayerLedgeFallingState::EnterState()
{
	Super::EnterState();

	if (StateController->PreviousState->IsA(UPlayerStandardMoveState::StaticClass()))
	{
		CanJump = true;
		CharacterController->OnJumpRequested.AddDynamic(this, &ThisClass::HandleJumpRequest);
		MainCharacter->JumpMaxCount = 2;
		FTimerManager& TM = GetWorld()->GetTimerManager();
		TM.SetTimer(CoyoteTimeTH, this, &ThisClass::HandleCoyoteTimeOver, CoyoteTime, false);
	}
	else
		CanJump = false;
	
	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: Ledge Falling State"));
}

void UPlayerLedgeFallingState::ExitState()
{
	Super::ExitState();

	CharacterController->OnJumpRequested.RemoveDynamic(this, &ThisClass::HandleJumpRequest);
	
	if (!StateController->NextState->IsA(UPlayerJumpState::StaticClass()))
		MainCharacter->JumpMaxCount = 1;
}

void UPlayerLedgeFallingState::HandleJumpRequest()
{
	if (!CanJump)
		return;

	CharacterController->OnJumpRequested.RemoveDynamic(this, &ThisClass::HandleJumpRequest);
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.ClearTimer(CoyoteTimeTH);
	StateTransition(JumpState);
}

void UPlayerLedgeFallingState::HandleCoyoteTimeOver()
{
	CanJump = false;
	MainCharacter->JumpMaxCount = 1;
}
