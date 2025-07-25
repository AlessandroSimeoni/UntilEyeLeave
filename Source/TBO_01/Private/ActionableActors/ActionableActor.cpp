// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/ActionableActor.h"

AActionableActor::AActionableActor()
{
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MyRoot->SetMobility(EComponentMobility::Static);
	RootComponent = MyRoot;
}

void AActionableActor::ActivateActor_Implementation(const bool InstantActivation)
{	
}

void AActionableActor::DeActivateActor_Implementation(const bool InstantDeActivation)
{
}

void AActionableActor::RequestActivation(const bool InstantActivation)
{
	// increment activation requests
	CurrentActivationRequests++;

	// safety clamp
	if (CurrentActivationRequests > ActivationRequestsRequired)
		CurrentActivationRequests = ActivationRequestsRequired;

	// check and activation
	if (CurrentActivationRequests == ActivationRequestsRequired)
	{
		Activated = true;
		ActivateActor(InstantActivation);
	}
}

void AActionableActor::RequestDeActivation(const bool InstantDeActivation)
{
	//decrease activation requests
	CurrentActivationRequests--;

	// safety clamp
	if (CurrentActivationRequests < 0)
		CurrentActivationRequests = 0;

	// if actor was already activated, then de-activate it
	if (Activated)
	{
		Activated = false;
		DeActivateActor(InstantDeActivation);
	}
}

