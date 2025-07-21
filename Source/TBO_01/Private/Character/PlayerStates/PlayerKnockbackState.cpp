// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStates/PlayerKnockbackState.h"

#include "Character/MainCharacter.h"
#include "Character/PlayerStates/PlayerIdleState.h"
#include "FSM/StateController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerKnockbackState::InitState(UStateController* Controller)
{
	Super::InitState(Controller);

	IdleState =  Cast<UPlayerIdleState>(StateController->FindStateOfClass(IdleStateClass));
}

void UPlayerKnockbackState::EnterState()
{
	Super::EnterState();
	
	UE_LOG(LogTemp, Log, TEXT("PLAYER STATE: Knockback State"));
	StartKnockback();
}

void UPlayerKnockbackState::EndKnockback()
{
	UE_LOG(LogTemp, Log, TEXT("Exit Knockback State"));
	StateTransition(IdleState);
}

void UPlayerKnockbackState::StartKnockback_Implementation() {}
