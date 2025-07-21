// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/BallisticProjectile.h"

void ABallisticProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CanShoot)
		return;

	if (SphereDetect())
		HandleCollision();
	
	SetActorLocation(GetActorLocation() + ProjectileVelocity * DeltaTime + 0.5f * FVector(0, 0, -Gravity) * DeltaTime * DeltaTime);
	SetActorRotation(ProjectileVelocity.Rotation());
	
	ProjectileVelocity.Z -= Gravity * DeltaTime;
}

void ABallisticProjectile::Shoot(const UMuzzle* InstigatorMuzzle, const float ProjectileSpeed, const AActor* ProjectileTargetActor)
{
	if (!ProjectileTargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No ProjectileTargetActor for ballistic projectile!"));
		return;
	}

	Super::Shoot(InstigatorMuzzle, ProjectileSpeed, ProjectileTargetActor);
	
	Gravity = -GetWorld()->GetGravityZ();
	TargetActor = ProjectileTargetActor;
	Speed = ProjectileSpeed;

	// calculate velocity
	FindProjectileDirectionNoGravity();
	
	// enable movement in tick
	CanShoot = true;
}

bool ABallisticProjectile::FindProjectileDirectionNoGravity()
{
	if (Speed <= 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("No solution for non-positive projectile speeds"));
		return false;
	}

	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector TargetVelocity = TargetActor->GetVelocity();
	
	
	// Calculate relative location
	const FVector DeltaLocation = TargetLocation - GetActorLocation();
	if (DeltaLocation.IsNearlyZero())
	{
		ResultLocation = TargetLocation;
		ProjectileVelocity = TargetVelocity;
		return true;
	}

	// Quadratic parameters
	const float A = DeltaLocation.SizeSquared();
	const float B = 2.f * FVector::DotProduct(TargetVelocity, DeltaLocation);
	const float C = TargetVelocity.SizeSquared() - Speed * Speed;

	// Square root delta sanity check
	float Delta = B * B - 4.f * A * C;
	if (Delta < 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("No solution due to negative delta"));
		return false;
	}

	// Quadratic solutions
	const float Sqrt = sqrt(Delta);
	const float InverseTime1 = 0.5f * (-B + Sqrt) / A;
	const float InverseTime2 = 0.5f * (-B - Sqrt) / A;

	// Actual solutions
	const float Time1 = InverseTime1 == 0.f ? -1.f : 1.f / InverseTime1;
	const float Time2 = InverseTime2 == 0.f ? -1.f : 1.f / InverseTime2;

	// Impossible solutions
	if (Time1 < 0.f && Time2 < 0.f)
	{
		UE_LOG(LogTemp, Log, TEXT("No solution due to negative times"));
		return false;
	}

	// Chosen time is the smallest positive
	float DeltaTime = Time1;
	if (Time1 < 0.f)
	{
		DeltaTime = Time2;
	}
	else if (Time2 >= 0.f)
	{
		DeltaTime = FMath::Min(Time1, Time2);
	}

	// Find location and result
	
		FVector PredictedLocation = TargetLocation + TargetVelocity * DeltaTime;
		ResultLocation = FMath::Lerp(PredictedLocation, TargetLocation, PredictionImprecision);
	
	
	FVector DesiredDirection = (ResultLocation - GetActorLocation()).GetSafeNormal();

	
	FVector ForwardDirection = GetActorForwardVector();
	float AngleDegrees = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardDirection, DesiredDirection)));

	if (AngleDegrees > MaxAngle)
	{
		
		FVector Axis = FVector::CrossProduct(ForwardDirection, DesiredDirection).GetSafeNormal();
	
		FQuat LimitQuat = FQuat(Axis, FMath::DegreesToRadians(MaxAngle));
		FVector LimitedDirection = LimitQuat.RotateVector(ForwardDirection);
		
		float Distance = (ResultLocation - GetActorLocation()).Size();
		ResultLocation = GetActorLocation() + LimitedDirection * Distance;
	}
	
	ProjectileVelocity = (ResultLocation - GetActorLocation()) / DeltaTime;
	
	return true;
}
