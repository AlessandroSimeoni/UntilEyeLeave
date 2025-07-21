// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionableActors/ActuatorActor.h"

#include "ActionableActors/ActionableActor.h"
#include "SaveSystem/SaveSubsystem.h"
#include "SaveSystem/TBOSaveGame.h"
#include "SaveSystem/UniqueIDComponent.h"


AActuatorActor::AActuatorActor()
{
	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MyRoot;
	
	UniqueIDComp = CreateDefaultSubobject<UUniqueIDComponent>(TEXT("UniqueID"));
}

void AActuatorActor::BeginPlay()
{
	Super::BeginPlay();
	
	SaveSubsystem = GetGameInstance()->GetSubsystem<USaveSubsystem>();
	UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame();
	if (TBOSaveGame->FindInteractedActor(UniqueIDComp->UniqueID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Saved restart"));
		ActionActors(true);
		Execute_RestoreState(this);
	}
}

void AActuatorActor::RestoreState_Implementation()
{
	OnRestore();
}

bool AActuatorActor::HasBeenActivated_Implementation()
{
	return Activated;
}

void AActuatorActor::ActionActors(const bool InstantActivation)
{
	for (const TObjectPtr<AActionableActor> Actionable : ConnectedActionableActor)
		if (Actionable)
		{
			Actionable->RequestActivation(InstantActivation);
			UE_LOG(LogTemp, Warning, TEXT("Action Actors"));
		}
	
	UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame();
	TBOSaveGame->AddInteractedActor(UniqueIDComp->UniqueID);

	Activated = true;
}

void AActuatorActor::DeActivateActors(const bool InstantDeActivation)
{
	for (const TObjectPtr<AActionableActor> Actionable : ConnectedActionableActor)
		if (Actionable)
			Actionable->RequestDeActivation(InstantDeActivation);

	UTBOSaveGame* TBOSaveGame = SaveSubsystem->LoadGame();
	TBOSaveGame->RemoveInteractedActor(UniqueIDComp->UniqueID);
	
	Activated = false;
}
