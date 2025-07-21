// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	RootComponent->SetMobility(EComponentMobility::Movable);
	
	TopPosition = CreateDefaultSubobject<USceneComponent>(TEXT("TopPosition"));
	TopPosition->SetupAttachment(RootComponent);

	BottomPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BottomPosition"));
	BottomPosition->SetupAttachment(RootComponent);

	MovingPart = CreateDefaultSubobject<USceneComponent>(TEXT("MovingPart"));
	MovingPart->SetupAttachment(RootComponent);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(MovingPart);
}

void AMovingPlatform::AttachActorsToPlatform()
{
	if(!MovingPart)
		return;
	
	for (AActor* Actor : AttachedActors)
		if (Actor)
			Actor->AttachToComponent(MovingPart, FAttachmentTransformRules::KeepWorldTransform);
}

void AMovingPlatform::InstantStateChange(bool Up)
{
	if(!MovingPart)
		return;
	
	CurrentlyUp = Up;

	if (Up)
		MovingPart->SetWorldLocation(TopPosition->GetComponentLocation());
	else
		MovingPart->SetWorldLocation(BottomPosition->GetComponentLocation());
}

void AMovingPlatform::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttachActorsToPlatform();
	InstantStateChange(IsUp);
}

void AMovingPlatform::ActivateActor_Implementation(bool InstantActivation)
{
}

void AMovingPlatform::DeActivateActor_Implementation(bool InstantDeActivation)
{
}

void AMovingPlatform::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

	for (AActor* Actor : AttachedActors)
	{
		if (!Actor) continue;
		
		if (Actor->GetRootComponent() && 
			Actor->GetRootComponent()->Mobility != EComponentMobility::Movable)
		{
			Actor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
		}

		
		TArray<UActorComponent*> Components;
		Actor->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Component);
			if (PrimComp && PrimComp->Mobility != EComponentMobility::Movable)
			{
				PrimComp->SetMobility(EComponentMobility::Movable);
			}
		}
	}
}