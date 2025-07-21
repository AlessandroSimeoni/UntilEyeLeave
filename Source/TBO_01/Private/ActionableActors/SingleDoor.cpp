// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/SingleDoor.h"

ASingleDoor::ASingleDoor()
{
	RootComponent->SetMobility(EComponentMobility::Movable);
	
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(RootComponent);
	
	DoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorPivot"));
	DoorPivot->SetupAttachment(RootComponent);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorPivot);
}

void ASingleDoor::InstantStateChange(bool Open)
{
	CurrentlyOpen = Open;
	DoorPivot->SetRelativeRotation(FRotator(0, Open ? -90.0f : 0.0f, 0));
}

void ASingleDoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InstantStateChange(IsOpen);
}

void ASingleDoor::ActivateActor_Implementation(bool InstantActivation)
{
}

void ASingleDoor::DeActivateActor_Implementation(bool InstantDeActivation)
{
}
