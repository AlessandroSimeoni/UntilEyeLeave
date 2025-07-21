// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/BackstabCreatureProjectile.h"

#include "NiagaraComponent.h"
#include "Creatures/Projectiles/AOEArea.h"
#include "Interface/HittableActor.h"
#include "Utility/EffectData.h"

void ABackstabCreatureProjectile::HandleCollision()
{
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.ClearTimer(CollisionTimeoutTH);
	
	CanShoot = false;
	
	AActor* HitActor = TraceHit.GetActor();
	if (HitActor)
	{
		if (HitActor->Implements<UHittableActor>())
		{
			const TScriptInterface<IHittableActor> HittableInstance = HitActor;
			HittableInstance->Execute_Hit(HitActor, DestructionLevel, Damage, this,KnockbackIntensityParameter);
		}
	}

	if (AOEAreaClass)
	{
		const FVector AOELineLocation = GetActorLocation() +
							 GetActorForwardVector() * AOELineStartOffset.X +
							 GetActorUpVector() * AOELineStartOffset.Z +
							 GetActorRightVector() * AOELineStartOffset.Y;
		
		GetWorld()->LineTraceSingleByChannel(AOEHit,
										  AOELineLocation,
										  AOELineLocation + FVector::DownVector * AOELineTraceDistance,
										  ECC_Visibility,
											  CollisionParams);


		FVector Location = AOEHit.Location;
		Location.Z = AOEHit.Location.Z + 50;
		GetWorld()->SpawnActor<AAOEArea>(AOEAreaClass,	Location,  FRotator::ZeroRotator);
	}

	CollisionFX.SpawnEffect(GetWorld(),GetActorLocation());
	/*
	if (CollisionEffect)
	{
		NiagaraComponent->SetAsset(CollisionEffect);
		NiagaraComponent->Activate(true);
		NiagaraComponent->OnSystemFinished.AddDynamic(this, &ThisClass::HandleCollisionEffectOver);
	}
	else
	*/
		EndProjectileLifeCycle();
}
