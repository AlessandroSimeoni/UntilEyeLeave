// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerStandardMoveState.h"

#include "Character/CharacterAnimInstance.h"
#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerCombatState.h"
#include "Character/PlayerStates/PlayerIdleState.h"
#include "Character/PlayerStates/PlayerJumpState.h"
#include "Character/PlayerStates/PlayerKnockbackState.h"
#include "Character/PlayerStates/PlayerLedgeFallingState.h"
#include "Character/PlayerStates/PlayerLedgeGrabState.h"
#include "Character/PlayerStates/PlayerRollState.h"
#include "Components/CapsuleComponent.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerStandardMoveState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	IdleState = Cast<UPlayerIdleState>(StateController->FindStateOfClass(IdleStateClass));
	LedgeFallingState = Cast<UPlayerLedgeFallingState>(StateController->FindStateOfClass(LedgeFallingStateClass));
	JumpState = Cast<UPlayerJumpState>(StateController->FindStateOfClass(JumpStateClass));
	OtherMoveModeState = Cast<UPlayerStandardMoveState>(StateController->FindStateOfClass(OtherMoveModeStateClass));
	RollState = Cast<UPlayerRollState>(StateController->FindStateOfClass(RollStateClass));
	LedgeGrabState = Cast<UPlayerLedgeGrabState>(StateController->FindStateOfClass(LedgeGrabStateClass));
	CombatState = Cast<UPlayerCombatState>(StateController->FindStateOfClass(CombatStateClass));
	KnockbackState = Cast<UPlayerKnockbackState>(StateController->FindStateOfClass(KnockbackStateClass));

	TM = &GetWorld()->GetTimerManager();
}

void UPlayerStandardMoveState::EnterState()
{
	Super::EnterState();

	MainCharacter->ToggleInteraction(true);
	
	SetCharacterMaxWalkSpeed();

	CharacterAnimInstance->StartIdleToRunInterpolation(IsSlowMovementState ? 0.5f : 1.0f); 

	MainCharacter->OnKnockbackRequested.AddDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnMoveInput.AddDynamic(this, &ThisClass::AddMovementDirection);
	CharacterController->OnJumpRequested.AddDynamic(this, &ThisClass::GoToJumpState);
	CharacterController->OnRollRequested.AddDynamic(this, &ThisClass::GoToRollState);
	CharacterController->OnAttackRequested.AddDynamic(this, &ThisClass::GoToCombatState);
	
	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: %s Move State"), IsSlowMovementState ? TEXT("SLOW") : TEXT(""));
}


void UPlayerStandardMoveState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	SquaredSpeed = MainCharacter->GetVelocity().SquaredLength();
	
	if (MainCharacter->MovementComponent->IsFalling())
	{
		StateTransition(LedgeFallingState);
		return;
	}
		
	if (MovementDirection == FVector::ZeroVector && SquaredSpeed == 0.0f)
	{
		StateTransition(IdleState);
		return;	
	}

	if ((!IsSlowMovementState && CharacterController->SlowMovement) || (IsSlowMovementState && !CharacterController->SlowMovement))
	{
		StateTransition(OtherMoveModeState);
		return;
	}
	
	if (CanLedgeGrab &&
		MainCharacter->CanLedgeGrab(VerticalRayUpwardOffset, VerticalRayForwardOffset,
					VerticalRayLength, VerticalRayRadius,
					HorizontalRayUpwardOffset, HorizontalRayLength,
					HorizontalRayRadius, PelvisFromLedgeMaxOffset,
				  LedgeGrabState->HorizontalLedgeHitResult,
					LedgeGrabState->VerticalLedgeHitResult)
		&& LedgeGrabState->CheckTargetGroundNearby())
	{
		if (FVector::DotProduct(MovementDirection, -LedgeGrabState->HorizontalLedgeHitResult.Normal) >= WallNormalToMoveDirectionDotThreshold)
		{
			const FVector CharacterGroundPosition = MainCharacter->GetActorLocation() - MainCharacter->GetActorUpVector() * MainCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			const float StepHeight = FMath::Abs(LedgeGrabState->VerticalLedgeHitResult.Location.Z - CharacterGroundPosition.Z);
			
			if (StepHeight <= MaxStepHeight)
				StateTransition(LedgeGrabState);
		}
	}

	ConsumeMovement();
}

void UPlayerStandardMoveState::ExitState()
{
	Super::ExitState();

	CharacterAnimInstance->StartIdleToRunInterpolation(0.0f); 
	TM->ClearTimer(AbruptDirectionTimer);

	ResetCharacterRotationRate();

	MainCharacter->OnKnockbackRequested.RemoveDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnMoveInput.RemoveDynamic(this, &ThisClass::AddMovementDirection);
	CharacterController->OnJumpRequested.RemoveDynamic(this, &ThisClass::GoToJumpState);
	CharacterController->OnRollRequested.RemoveDynamic(this, &ThisClass::GoToRollState);
	CharacterController->OnAttackRequested.RemoveDynamic(this, &ThisClass::GoToCombatState);
}

void UPlayerStandardMoveState::AddMovementDirection(FVector Direction)
{
	if (!TM->IsTimerActive(AbruptDirectionTimer) && AbruptDirectionWaitTime > 0 && FVector::DotProduct(LastMovementDirection, Direction) < AbruptDirectionDetectionThreshold)
	{
		
		MainCharacter->MovementComponent->RotationRate = FRotator(0, AbruptDirectionYawRotationRate, 0);
		CharacterAnimInstance->StartIdleToRunInterpolation(0.0f); 
		MainCharacter->MovementComponent->MaxWalkSpeed = 0.0f;
		
		TM->SetTimer(AbruptDirectionTimer, this, &ThisClass::RestartMovingAfterWait, AbruptDirectionWaitTime, false);
	}
	
	MovementDirection += Direction;
	LastMovementDirection = MovementDirection;
}

void UPlayerStandardMoveState::ConsumeMovement()
{
	MainCharacter->AddMovementInput(MovementDirection);
	MovementDirection = FVector::ZeroVector;
}

void UPlayerStandardMoveState::RestartMovingAfterWait() const
{
	ResetCharacterRotationRate();
	SetCharacterMaxWalkSpeed();
	CharacterAnimInstance->StartIdleToRunInterpolation(IsSlowMovementState ? 0.5f : 1.0f); 
}

void UPlayerStandardMoveState::GoToJumpState()
{
	StateTransition(JumpState);
}

void UPlayerStandardMoveState::GoToRollState()
{
	StateTransition(RollState);
}

void UPlayerStandardMoveState::GoToCombatState()
{
	StateTransition(CombatState);
}

void UPlayerStandardMoveState::GoToKnockbackState()
{
	StateTransition(KnockbackState);
}

void UPlayerStandardMoveState::SetCharacterMaxWalkSpeed() const
{
	MainCharacter->MovementComponent->MaxWalkSpeed = MaxWalkSpeed;
}

void UPlayerStandardMoveState::ResetCharacterRotationRate() const
{
	MainCharacter->SetRotationRateOfDevice(CharacterController->RecentlyUsedDevice);
}
