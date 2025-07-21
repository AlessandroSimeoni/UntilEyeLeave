// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/StopLight.h"

AStopLight::AStopLight()
{
	StopLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StopLightMesh"));
	StopLightMesh->SetupAttachment(RootComponent);
}

void AStopLight::ActivateActor_Implementation(bool InstantActivation)
{
}

void AStopLight::DeActivateActor_Implementation(bool InstantDeActivation)
{
}
