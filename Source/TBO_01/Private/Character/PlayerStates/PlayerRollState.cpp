// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerRollState.h"

#include "Character/CharacterAnimInstance.h"
#include "Character/CharacterController.h"
#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerIdleState.h"
#include "Character/PlayerStates/PlayerJumpState.h"
#include "Character/PlayerStates/PlayerKnockbackState.h"
#include "Character/PlayerStates/PlayerLedgeFallingState.h"
#include "Components/CapsuleComponent.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerRollState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);
	
	IdleState = Cast<UPlayerIdleState>(StateController->FindStateOfClass(IdleStateClass));
	FallingLedgeState = Cast<UPlayerLedgeFallingState>(StateController->FindStateOfClass(FallingLedgeStateClass));
	KnockbackState = Cast<UPlayerKnockbackState>(StateController->FindStateOfClass(KnockbackStateClass));
	JumpState = Cast<UPlayerJumpState>(StateController->FindStateOfClass(JumpStateClass));
	
	DefaultCharacterCapsuleHalfHeight = MainCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	CollisionParams.AddIgnoredActor(MainCharacter);
}

void UPlayerRollState::EnterState()
{
	Super::EnterState();

	MainCharacter->ToggleInteraction(false);
	
	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: Roll State"));

	if (RollCooldown > 0)
		StateReady = false;

	if (StateController->PreviousState != IdleState)
	{
		// instantly rotate character towards roll direction 
		FRotator TargetRotation = FRotationMatrix::MakeFromX(CharacterController->MovementDirection).Rotator();
		MainCharacter->SetActorRotation(TargetRotation);
	}

	MainCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(RollCapsuleHalfHeight);
	MainCharacter->GetMesh()->AddLocalOffset(FVector(0, 0, CharacterMeshOffset));
	MainCharacter->OnKnockbackRequested.AddDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnJumpRequested.AddDynamic(this, &ThisClass::GoToJumpState);
	StartRolling();
}

void UPlayerRollState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	
	if (!IsUnderObject)
		return;

	// check if there is an obstacle in front of the player while under an object 
	if (!FrontWallObstacle)
	{
		FrontWallObstacle = GetWorld()->SweepSingleByChannel(HitResult,
														MainCharacter->GetActorLocation(),
														MainCharacter->GetActorLocation() + MainCharacter->GetActorForwardVector() * FrontWallSphereDetectionLength,
														FQuat::Identity,
														ECC_Visibility,
														FCollisionShape::MakeSphere(FrontWallSphereDetectionRadius),
														CollisionParams);
	}

	// calculate the escape direction (FrontWallCrossResult) to avoid getting stuck under the object
	if (FrontWallObstacle && FrontWallCrossResult == FVector::ZeroVector)
	{
		FrontWallCrossResult = FVector::CrossProduct(FVector::UpVector,HitResult.Normal);
		MainCharacter->SetActorLocation(MainCharacter->GetActorLocation() + HitResult.Normal * (SphereCastRadius + FrontWallSafeOffset));

		FRotator Rotation = FRotator(0, -UnderObjectEscapeDegrees, 0);
		if (FVector::DotProduct(FrontWallCrossResult, MainCharacter->GetActorForwardVector()) >= 0.0f)
		{
			FrontWallCrossResult *= -1;
			Rotation = FRotator(0, UnderObjectEscapeDegrees, 0);
		}
		
		FrontWallCrossResult = Rotation.RotateVector(FrontWallCrossResult);
	}

	// move the player while under an object
	MainCharacter->SetActorLocation(MainCharacter->GetActorLocation() + (FrontWallObstacle ? FrontWallCrossResult : MainCharacter->GetActorForwardVector()) * SlideSpeedUnderObjects * DeltaTime);
	TryEndRolling();
}

void UPlayerRollState::ExitState()
{
	Super::ExitState();
	
	if (RollCooldown > 0)
	{
		FTimerManager& TM = GetWorld()->GetTimerManager();
		TM.SetTimer(RollCooldownTimer, this, &ThisClass::EnableRoll, RollCooldown, false);
	}

	// this is to avoid ground interpenetration 
	if (!MainCharacter->MovementComponent->IsFalling())
		MainCharacter->SetActorLocation(MainCharacter->GetActorLocation() + FVector::UpVector*RollCapsuleHalfHeight);

	MainCharacter->SetInvulnerability(false);
	MainCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCharacterCapsuleHalfHeight);
	MainCharacter->GetMesh()->AddLocalOffset(FVector(0, 0, -CharacterMeshOffset));
	FrontWallCrossResult = FVector::ZeroVector;
	FrontWallObstacle = false;
	MainCharacter->OnKnockbackRequested.RemoveDynamic(this, &ThisClass::GoToKnockbackState);
	CharacterController->OnJumpRequested.RemoveDynamic(this, &ThisClass::GoToJumpState);
}

void UPlayerRollState::StartRolling_Implementation() { }

bool UPlayerRollState::CheckObstacleAboveHead()
{
	SphereCastStart = MainCharacter->GetActorLocation();
	SphereCastEnd = SphereCastStart + MainCharacter->GetActorUpVector() * (RollCapsuleHalfHeight + DefaultCharacterCapsuleHalfHeight);
	
	return GetWorld()->SweepSingleByChannel(HitResult,
                                                 SphereCastStart,
                                                 SphereCastEnd,
                                                 FQuat::Identity,
                                                 ECC_Visibility,
                                                 FCollisionShape::MakeSphere(SphereCastRadius),
                                                 CollisionParams);
}

void UPlayerRollState::TryEndRolling()
{	
	if (MainCharacter->MovementComponent->IsFalling())
	{
		StateTransition(FallingLedgeState);
		return;
	}

	IsUnderObject = CheckObstacleAboveHead();

	if (!IsUnderObject)
		StateTransition(IdleState);
}

void UPlayerRollState::EnableRoll()
{
	StateReady = true;
}

void UPlayerRollState::GoToKnockbackState()
{
	CharacterAnimInstance->Montage_Stop(ToKnockbackBlendingTime, RollAnimMontage);
	StateTransition(KnockbackState);
}

void UPlayerRollState::GoToJumpState()
{
	if (CheckObstacleAboveHead())
		return;
	
	CharacterAnimInstance->Montage_Stop(ToJumpBlendingTime, RollAnimMontage);
	StateTransition(JumpState);
}
