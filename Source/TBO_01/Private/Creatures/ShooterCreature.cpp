// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/ShooterCreature.h"

#include "Creatures/Projectiles/MuzzlesController.h"

AShooterCreature::AShooterCreature()
{
	MuzzleController = CreateDefaultSubobject<UMuzzlesController>("MuzzlesController");
}

UMuzzlesController* AShooterCreature::GetMuzzlesController_Implementation()
{
	return MuzzleController;
}
