// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerMidAirState.h"

#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerCombatState.h"
#include "Character/PlayerStates/PlayerIdleState.h"
#include "Character/PlayerStates/PlayerKnockbackState.h"
#include "Character/PlayerStates/PlayerLedgeGrabState.h"
#include "Character/PlayerStates/PlayerRollState.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"


void UPlayerMidAirState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	IdleState =  Cast<UPlayerIdleState>(StateController->FindStateOfClass(IdleStateClass));
	LedgeGrabState =  Cast<UPlayerLedgeGrabState>(StateController->FindStateOfClass(LedgeGrabStateClass));
	CombatState =  Cast<UPlayerCombatState>(StateController->FindStateOfClass(CombatStateClass));
	KnockbackState =  Cast<UPlayerKnockbackState>(StateController->FindStateOfClass(KnockbackStateClass));
	RollState = Cast<UPlayerRollState>(StateController->FindStateOfClass(RollStateClass));
}


void UPlayerMidAirState::EnterState()
{
	Super::EnterState();

	MainCharacter->ToggleInteraction(false);
	MainCharacter->MovementComponent->bOrientRotationToMovement = false;
	PreviousMaxWalkSpeed = MainCharacter->MovementComponent->MaxWalkSpeed;
	MainCharacter->MovementComponent->MaxWalkSpeed = MidAirMaxSpeed;
	MainCharacter->MovementComponent->GravityScale = FallingGravityScale;
	MainCharacter->OnKnockbackRequested.AddDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnMoveInput.AddDynamic(this, &ThisClass::HandleMidAirMovement);
	CharacterController->OnAttackRequested.AddDynamic(this, &ThisClass::HandleMidAirAttack);
	CharacterController->OnRollRequested.AddDynamic(this, &ThisClass::HandleRollRequest);

	if (BeginMidAirSFX)
	{
		UGameplayStatics::PlaySound2D(this, BeginMidAirSFX);
	}
}

void UPlayerMidAirState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	
	if (!MainCharacter->MovementComponent->IsFalling())
	{
		if (RollRequested)
			StateTransition(RollState);
		else
			StateTransition(IdleState);
		
		return;
	}

	if (((FallingOnlyLedgeGrab && MainCharacter->GetVelocity().Z < 0) || !FallingOnlyLedgeGrab) &&
		MainCharacter->CanLedgeGrab(VerticalRayUpwardOffset, VerticalRayForwardOffset,
					VerticalRayLength,VerticalRayRadius,
				       0.0f, HorizontalRayLength,
				       HorizontalRayRadius, PelvisFromLedgeMaxOffset,
				     LedgeGrabState->HorizontalLedgeHitResult,
				       LedgeGrabState->VerticalLedgeHitResult))
	{
		StateTransition(LedgeGrabState);
	}
}

void UPlayerMidAirState::ExitState()
{
	Super::ExitState();

	if (!StateController->NextState->IsA(UPlayerCombatState::StaticClass()))
	{
		MainCharacter->MovementComponent->MaxWalkSpeed = PreviousMaxWalkSpeed;
		MainCharacter->MovementComponent->GravityScale = 1.0f;
	}

	MainCharacter->MovementComponent->bOrientRotationToMovement = true;
	CombatState->SetAttackReady();
	MainCharacter->OnKnockbackRequested.RemoveDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnMoveInput.RemoveDynamic(this, &ThisClass::HandleMidAirMovement);
	CharacterController->OnAttackRequested.RemoveDynamic(this, &ThisClass::HandleMidAirAttack);
	CharacterController->OnRollRequested.RemoveDynamic(this, &ThisClass::HandleRollRequest);
	
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.ClearTimer(RollInputTH);
	RollRequested = false;

	if (EndMidAirSFX && StateController->NextState != LedgeGrabState)
	{
		UGameplayStatics::PlaySound2D(this, EndMidAirSFX);
	}
}

void UPlayerMidAirState::HandleMidAirMovement(FVector MovementDirection)
{
	ForwardMovementComponent = MovementDirection.ProjectOnTo(MainCharacter->GetActorForwardVector()) * ForwardControlFactor;
	HorizontalMovementComponent = (MovementDirection - ForwardMovementComponent) * HorizontalControlFactor;

	MainCharacter->AddMovementInput(ForwardMovementComponent + HorizontalMovementComponent);
}

void UPlayerMidAirState::HandleMidAirAttack()
{
	CombatState->ForwardMidAirControlFactor = ForwardControlFactor;
	CombatState->HorizontalMidAirControlFactor = HorizontalControlFactor;
	StateTransition(CombatState);
}

void UPlayerMidAirState::GoToKnockbackState()
{
	StateTransition(KnockbackState);
}

void UPlayerMidAirState::HandleRollRequest()
{
	RollRequested = true;

	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(RollInputTH, this, &ThisClass::CancelRollRequest, RollInputTimeout, false);
}

void UPlayerMidAirState::CancelRollRequest()
{
	RollRequested = false;
}
