// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/BaseProjectile.h"
#include "NiagaraComponent.h"
#include "Creatures/Projectiles/Muzzle.h"
#include "Interface/HittableActor.h"
#include "Utility/EffectData.h"

#define COLLISION_PROJECTILE ECC_GameTraceChannel5
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	RootComponent = NiagaraComponent;
}

void ABaseProjectile::Shoot(const UMuzzle* InstigatorMuzzle, const float ProjectileSpeed, const AActor* ProjectileTargetActor)
{
	// ignore collision with the owner of the muzzle
	CollisionParams.AddIgnoredActor(InstigatorMuzzle->GetOwner());
	
	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(CollisionTimeoutTH, this, &ThisClass::EndProjectileLifeCycle, CollisionTimeout, false);
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionParams.AddIgnoredActor(this);
}

void ABaseProjectile::HandleCollisionEffectOver(UNiagaraComponent* PSystem)
{
	EndProjectileLifeCycle();
}

void ABaseProjectile::HandleCollision()
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

void ABaseProjectile::EndProjectileLifeCycle()
{
	OnProjectileEndLifeCycle.Broadcast();
	Destroy();
}

bool ABaseProjectile::SphereDetect()
{
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);
	const FVector SphereLocation = GetActorLocation() +
							 GetActorForwardVector() * SphereStartOffset.X +
							 GetActorUpVector() * SphereStartOffset.Z +
							 GetActorRightVector() * SphereStartOffset.Y;

	const bool Hit = GetWorld()->SweepSingleByChannel(
	   TraceHit, 
	   SphereLocation, 
	   SphereLocation + GetActorForwardVector() * SphereDistance, 
	   GetActorRotation().Quaternion(), 
	   COLLISION_PROJECTILE, 
	   Sphere,
	   CollisionParams
	);

		
	if (SphereDebug)
	{

		DrawDebugSphere(GetWorld(), SphereLocation, SphereRadius, 12, FColor::Red, false, 0.05f);
		DrawDebugSphere(GetWorld(), SphereLocation + GetActorForwardVector() * SphereDistance, SphereRadius, 12, FColor::Red, false, 0.05f);
	}

	return Hit;
}

bool ABaseProjectile::LineDetect()
{
	const FVector LineLocation = GetActorLocation() +
								 GetActorForwardVector() * LineStartOffset.X +
								 GetActorUpVector() * LineStartOffset.Z +
								 GetActorRightVector() * LineStartOffset.Y;

	const bool Hit = GetWorld()->LineTraceSingleByChannel(TraceHit,
	                                                      LineLocation,
	                                                      LineLocation + GetActorForwardVector() * LineDistance,
	                                                      COLLISION_PROJECTILE,
	                                                      CollisionParams);

	if (LineDebug)
		DrawDebugLine(GetWorld(), LineLocation, LineLocation + GetActorForwardVector() * LineDistance, FColor::Red, false, 0.01f);
	
	return Hit;
}
