// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/WeaponPiece.h"

#include "ActionableActors/Portal.h"

void AWeaponPiece::HandleCollectedStatus()
{
	ActionActors(true);
	WeaponTorchNotify();
	
	Super::HandleCollectedStatus();
}

void AWeaponPiece::ActionActors(const bool InstantActivation)
{
	for (const TObjectPtr<AActionableActor> Actionable : ConnectedActionableActors)
		if (Actionable)
			Actionable->RequestActivation(InstantActivation);
}
