// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerIdleState.h"

#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerCombatState.h"
#include "Character/PlayerStates/PlayerJumpState.h"
#include "Character/PlayerStates/PlayerKnockbackState.h"
#include "Character/PlayerStates/PlayerLedgeFallingState.h"
#include "Character/PlayerStates/PlayerRollState.h"
#include "Character/PlayerStates/PlayerSlowMoveState.h"
#include "Character/PlayerStates/PlayerStandardMoveState.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerIdleState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	StandardMoveState = Cast<UPlayerStandardMoveState>(StateController->FindStateOfClass(StandardMoveStateClass));
	SlowMoveState = Cast<UPlayerSlowMoveState>(StateController->FindStateOfClass(SlowMoveStateClass));
	JumpState = Cast<UPlayerJumpState>(StateController->FindStateOfClass(JumpStateClass));
	RollState = Cast<UPlayerRollState>(StateController->FindStateOfClass(RollStateClass));
	CombatState = Cast<UPlayerCombatState>(StateController->FindStateOfClass(CombatStateClass));
	KnockbackState = Cast<UPlayerKnockbackState>(StateController->FindStateOfClass(KnockbackStateClass));
	LedgeFallingState = Cast<UPlayerLedgeFallingState>(StateController->FindStateOfClass(LedgeFallingStateClass));
}

void UPlayerIdleState::EnterState()
{
	Super::EnterState();

	MainCharacter->ToggleInteraction(true);

	MainCharacter->OnKnockbackRequested.AddDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnMoveInput.AddDynamic(this, &ThisClass::MovementStateTransition);
	CharacterController->OnJumpRequested.AddDynamic(this, &ThisClass::GoToJumpState);
	CharacterController->OnRollRequested.AddDynamic(this, &ThisClass::GoToRollState);
	CharacterController->OnAttackRequested.AddDynamic(this, &ThisClass::GoToCombatState);

	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: Idle State"));
}

void UPlayerIdleState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	if (MainCharacter->MovementComponent->IsFalling())
		StateTransition(LedgeFallingState);
}

void UPlayerIdleState::ExitState()
{
	Super::ExitState();

	MainCharacter->OnKnockbackRequested.RemoveDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnMoveInput.RemoveDynamic(this, &ThisClass::MovementStateTransition);
	CharacterController->OnJumpRequested.RemoveDynamic(this, &ThisClass::GoToJumpState);
	CharacterController->OnRollRequested.RemoveDynamic(this, &ThisClass::GoToRollState);
	CharacterController->OnAttackRequested.RemoveDynamic(this, &ThisClass::GoToCombatState);
}

void UPlayerIdleState::MovementStateTransition(FVector Direction)
{
	if (!StandardMoveState)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Idle State: StandardMoveState is not valid!"));
		return;
	}

	if (!SlowMoveState)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Idle State: SlowMoveState is not valid!"));
		return;
	}
	
	if (CharacterController->SlowMovement)
	{
		SlowMoveState->MovementDirection = Direction;
		StateTransition(SlowMoveState);
	}
	else
	{
		StandardMoveState->MovementDirection = Direction;
		StateTransition(StandardMoveState);
	}
}

void UPlayerIdleState::GoToJumpState()
{
	StateTransition(JumpState);
}

void UPlayerIdleState::GoToRollState()
{
	StateTransition(RollState);
}

void UPlayerIdleState::GoToCombatState()
{
	StateTransition(CombatState);
}

void UPlayerIdleState::GoToKnockbackState()
{
	StateTransition(KnockbackState);
}
