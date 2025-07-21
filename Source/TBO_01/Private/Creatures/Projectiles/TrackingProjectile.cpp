// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/TrackingProjectile.h"


void ATrackingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanShoot)
		return;

	ElapsedTime += DeltaTime;

	if (SphereDetect())
	{
		HandleCollision();
		return;
	}

	if (ElapsedTime < StraightTrajectoryDuration)
	{
		
	}
	else
	{
		if (Target.IsValid())
		{
			FVector DirectionToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FRotator CurrentRotation = CurrentVelocity.Rotation();
			FRotator TargetRotation = DirectionToTarget.Rotation();

			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);
			FVector NewDirection = NewRotation.Vector();

			
			CurrentVelocity = NewDirection * CurrentVelocity.Size();
		}
		
	}

	
	if (GravityScale > 0.0f)
	{
		FVector GravityAccel(0.f, 0.f, GetWorld()->GetGravityZ() * GravityScale);
		CurrentVelocity += GravityAccel * DeltaTime;
	}

	
	FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
	SetActorLocation(NewLocation);

	
	SetActorRotation(CurrentVelocity.Rotation());
}
void ATrackingProjectile::Shoot(const UMuzzle* InstigatorMuzzle, float ProjectileSpeed,
	const AActor* ProjectileTargetActor)
{
	Super::Shoot(InstigatorMuzzle, ProjectileSpeed, ProjectileTargetActor);
	Super::Shoot(InstigatorMuzzle, ProjectileSpeed, ProjectileTargetActor);

	Target = const_cast<AActor*>(ProjectileTargetActor);
	Speed = ProjectileSpeed;
	CanShoot = true;
	CurrentVelocity = GetActorForwardVector() * Speed;
}
