// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/DoubleDoor.h"

ADoubleDoor::ADoubleDoor()
{
	RootComponent->SetMobility(EComponentMobility::Movable);

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	WallMesh->SetupAttachment(RootComponent);

	RightDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("RightDoorPivot"));
	RightDoorPivot->SetupAttachment(RootComponent);

	LeftDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("LeftDoorPivot"));
	LeftDoorPivot->SetupAttachment(RootComponent);

	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
	RightDoorMesh->SetupAttachment(RightDoorPivot);

	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	LeftDoorMesh->SetupAttachment(LeftDoorPivot);
}

void ADoubleDoor::InstantStateChange(bool Open)
{
	CurrentlyOpen = Open;
	RightDoorPivot->SetRelativeRotation(FRotator(0, Open ? -90.0f : 0.0f, 0));
	LeftDoorPivot->SetRelativeRotation(FRotator(0, Open ? 90.0f : 0.0f, 0));
}

void ADoubleDoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InstantStateChange(IsOpen);
}

void ADoubleDoor::ActivateActor_Implementation(bool InstantActivation)
{
}

void ADoubleDoor::DeActivateActor_Implementation(bool InstantDeActivation)
{
}
