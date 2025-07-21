// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/StraightProjectileExplosive.h"



void AStraightProjectileExplosive::HandleCollision()
{
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.ClearTimer(CollisionTimeoutTH);
	
	CanShoot = false;

	
	if (AOEExplosion)
	{
		AAOEDamage* Explosion = GetWorld()->SpawnActor<AAOEDamage>(AOEExplosion,	GetActorLocation(),  FRotator::ZeroRotator);
		Explosion->Explode(DestructionLevel,Damage,Size,KnockbackIntensityParameter);
	}
	
	EndProjectileLifeCycle();
}
