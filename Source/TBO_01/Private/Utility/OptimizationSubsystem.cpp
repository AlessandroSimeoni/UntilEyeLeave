// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/OptimizationSubsystem.h"

#include "AIController.h"
#include "Creatures/Creature.h"
#include "Creatures/CreatureAiController.h"
#include "Creatures/Senses/ComplexCreatureSight.h"
#include "Destructible/HittableDestructibleActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"


TStatId UOptimizationSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UOptimizationSubsystem, STATGROUP_Tickables);
}

void UOptimizationSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	constexpr float MaxDistanceTickSqrd = 2000.0f * 2000.0f;
	constexpr float MaxDistanceCreatureTickSqrd = 2800.0f * 2800.0f;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APawn* Pawn = PC->GetPawn();
	FVector PlayerLocation = Pawn ? Pawn->GetActorLocation(): FVector::ZeroVector;
	if (bTickDetstructible)
	{
		for (int32 i = HittableDestructibles.Num() -1; i >= 0; i--)
	{
		if (!HittableDestructibles[i].IsValid())
		{
			HittableDestructibles.RemoveAtSwap(i);
		}
		bool bIsInRange = (HittableDestructibles[i]->GetActorLocation() - PlayerLocation).SizeSquared() <= MaxDistanceTickSqrd;
		if (USkeletalMeshComponent* SkeletalMeshComponent = HittableDestructibles[i]->GetSkeletalMeshComponent())
		{
			if (bIsInRange != SkeletalMeshComponent->IsComponentTickEnabled())
			{
				SkeletalMeshComponent->SetComponentTickEnabled(bIsInRange);
			}
		}
	}
		
	}
else
{
	for (int32 i = Creatures.Num() -1; i >= 0; i--)
	{
		if (!Creatures[i].IsValid())
		{
			Creatures.RemoveAtSwap(i);
		}
		bool bIsInRange = (Creatures[i]->GetActorLocation() - PlayerLocation).SizeSquared() <= MaxDistanceCreatureTickSqrd;

		Creatures[i]->SetOptimizationTick(bIsInRange);
	}
	
	for (int32 i = EyesHandlerActorComponents.Num() -1; i >= 0; i--)
	{
		if (!EyesHandlerActorComponents[i].IsValid())
		{
			EyesHandlerActorComponents.RemoveAtSwap(i);
		}
		bool bIsInRange = (EyesHandlerActorComponents[i]->GetOwner()->GetActorLocation() - PlayerLocation).SizeSquared() <= MaxDistanceTickSqrd;

		EyesHandlerActorComponents[i]->SetOptimizationTick(bIsInRange);
	}
}
	bTickDetstructible = !bTickDetstructible;
	
}



void UOptimizationSubsystem::AddHittableDestructibleActor(AHittableDestructibleActor* Actor)
{
	AddToList(HittableDestructibles, Actor);
}

void UOptimizationSubsystem::RemoveHittableDestructibleActor(AHittableDestructibleActor* Actor)
{
	RemoveFromList(HittableDestructibles, Actor);
}

void UOptimizationSubsystem::AddCreature(ACreature* Creature)
{
	AddToList(Creatures, Creature);
}

void UOptimizationSubsystem::RemoveCreature(ACreature* Creature)
{
	RemoveFromList(Creatures, Creature);
}

void UOptimizationSubsystem::AddEyesHandlerActorComponent(UEyesHandlerActorComponent* Handler)
{
	AddToList(EyesHandlerActorComponents, Handler);
}

void UOptimizationSubsystem::RemoveEyesHandlerActorComponent(UEyesHandlerActorComponent* Handler)
{
	RemoveFromList(EyesHandlerActorComponents, Handler);
}

