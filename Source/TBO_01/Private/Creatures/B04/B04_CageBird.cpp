// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/B04/B04_CageBird.h"

#include "Components/CapsuleComponent.h"
#include "Creatures/B04/B04_Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Charge, "FeedBack.Charge")



void AB04_CageBird::BeginPlay()
{
	Super::BeginPlay();
	if (StartInFlight)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	if (StartCharged)
	{
		SelfCharge();
	}
	
	SetFree(StartFree);
}
void AB04_CageBird::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		GetCapsuleComponent()->SetCanEverAffectNavigation(false);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		GetCapsuleComponent()->SetCanEverAffectNavigation(true);
	}

}


//wait for 0.1 for begin play initialization
void AB04_CageBird::SelfCharge()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AB04_CageBird::UseAbilitySelfCharge, 0.1f, false);
}

void AB04_CageBird::SetFree(bool IsFree)
{
	if (IsFree)
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(TAG_FreeMove);
		
	}
	else
	{
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(TAG_FreeMove,10);
	}
}

void AB04_CageBird::UseAbilitySelfCharge()
{
	FGameplayEventData EventData;
	EventData.Instigator = this;
	AbilitySystemComponent->HandleGameplayEvent(FGameplayTag(TAG_Charge), &EventData);
	if (AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Charge)))
		UE_LOG(LogTemp,Error,TEXT("Gameplay Abilities by Tag Charge"));
}




