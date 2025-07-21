// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/B05Boss/B05_BossHand.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AB05_BossHand::SetRestPoint(FVector RestPoint)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(KeyNameRestPosition, RestPoint);
	}
}

void AB05_BossHand::SetActiveStatus(bool Status)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(KeyNameHandActive, Status);
		AIController->GetBlackboardComponent()->SetValueAsBool(KeyNameStop, !Status);
	}
	StatusActive = Status;
	OnStatusChange.Broadcast(StatusActive);
}

void AB05_BossHand::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		GetCapsuleComponent()->SetCanEverAffectNavigation(false);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
		GetCapsuleComponent()->SetCanEverAffectNavigation(true);
	}
	
}
