// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerLedgeGrabState.h"

#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerIdleState.h"
#include "Character/PlayerStates/PlayerMidAirState.h"
#include "Components/CapsuleComponent.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerLedgeGrabState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	IdleState =  Cast<UPlayerIdleState>(StateController->FindStateOfClass(IdleStateClass));
	
	OnInit();
}

void UPlayerLedgeGrabState::EnterState()
{
	Super::EnterState();

	MainCharacter->ToggleInteraction(false);
	MainCharacter->MovementComponent->SetMovementMode(MOVE_Flying);
	MainCharacter->MovementComponent->StopMovementImmediately();

	if (StateController->PreviousState->IsA(UPlayerMidAirState::StaticClass()))
		StartMidAirLedgeGrab();
	else
		StartGroundedLedgeGrab();

	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: Ledge Grab State"));
}

void UPlayerLedgeGrabState::ExitState()
{
	Super::ExitState();

	MainCharacter->MovementComponent->SetMovementMode(MOVE_Walking);
}

void UPlayerLedgeGrabState::EndLedgeGrab()
{
	// position the player to the correct height
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MainCharacter);
	const FVector Start = MainCharacter->GetActorLocation() + FVector::UpVector * 100.0f;
	const FVector End = Start + FVector::UpVector * (-200.0f);
	const bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	if (Hit)
		MainCharacter->SetActorLocation(HitResult.Location + FVector::UpVector * MainCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	
	StateTransition(IdleState);
}

bool UPlayerLedgeGrabState::CheckTargetGroundNearby() const
{
	/*
	 *  first, check if target ground is near the player by doing a raycast from the player feet location towards
	 *  the opposite direction of the normal detected by the horizontal ray of the ledge grab detection system.
	 *  This avoids grounded ledge grab from strange angles. 
	 */
		
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(MainCharacter);

	FVector TargetGroundRightVector = FVector::CrossProduct(MainCharacter->GetActorUpVector(), HorizontalLedgeHitResult.Normal);
	float Dot = FVector::DotProduct(TargetGroundRightVector, MainCharacter->GetActorForwardVector());
	
	const FVector TargetGroundCheckStartPos = MainCharacter->GetActorLocation() -
											  MainCharacter->GetActorUpVector()*(MainCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 5.0f) +
											  ((Dot <= 0.0f) ? TargetGroundRightVector : -TargetGroundRightVector)*MainCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius()*0.5f;
	
	const FVector TargetGroundCheckEndPos = TargetGroundCheckStartPos - HorizontalLedgeHitResult.Normal * GroundCheckBeforeClimbDistance;

	
	if (GetWorld()->LineTraceSingleByChannel(Hit, TargetGroundCheckStartPos, TargetGroundCheckEndPos, ECC_Visibility, CollisionParams))
	{
		if (Hit.GetActor() && Hit.GetActor()->ActorHasTag(MainCharacter->LedgeGrabTag))
		{
			//DrawDebugLine(GetWorld(), TargetGroundCheckStartPos, TargetGroundCheckEndPos, FColor::Red, false, 0.2f, 0, 2.0f);
			return true;
		}

		return false;
	}

	return false;
	
}

void UPlayerLedgeGrabState::OnInit_Implementation() {}
void UPlayerLedgeGrabState::StartMidAirLedgeGrab_Implementation() {}
void UPlayerLedgeGrabState::StartGroundedLedgeGrab_Implementation() {}
