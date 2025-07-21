// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerJumpState.h"

#include "Character/CharacterAnimInstance.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerCombatState.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerJumpState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);
	
	MainCharacter->MovementComponent->JumpZVelocity = FMath::Sqrt(JumpHeight * 2 * FallingGravityScale * FMath::Abs(GetWorld()->GetGravityZ()));
}

void UPlayerJumpState::EnterState()
{
	Super::EnterState();
	
	TargetForwardControl = StateController->PreviousState->IsA(IdleStateClass) ? FromIdleForwardControlFactor : ForwardControlFactor;
	TargetHorizontalControl = StateController->PreviousState->IsA(IdleStateClass) ? FromIdleHorizontalControlFactor : HorizontalControlFactor;
	
	CharacterAnimInstance->IsJumping = true;	
	MainCharacter->Jump();
	
	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: Jump State"));
}

void UPlayerJumpState::ExitState()
{
	Super::ExitState();

	CharacterAnimInstance->IsJumping = false;
	MainCharacter->JumpMaxCount = 1;
}

void UPlayerJumpState::HandleMidAirMovement(FVector MovementDirection)
{
	
	ForwardMovementComponent = MovementDirection.ProjectOnTo(MainCharacter->GetActorForwardVector()) * TargetForwardControl;
	HorizontalMovementComponent = (MovementDirection - ForwardMovementComponent) * TargetHorizontalControl;

	MainCharacter->AddMovementInput(ForwardMovementComponent + HorizontalMovementComponent);
}

void UPlayerJumpState::HandleMidAirAttack()
{
	CombatState->ForwardMidAirControlFactor = TargetForwardControl;
	CombatState->HorizontalMidAirControlFactor = TargetHorizontalControl;
	StateTransition(CombatState);
}
