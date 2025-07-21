// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Projectiles/MuzzlesController.h"

#include "Creatures/DataAsset/ShootSequence.h"
#include "Creatures/Projectiles/Muzzle.h"

void UMuzzlesController::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwner()->GetComponents<UMuzzle>(Muzzles);
}

TArray<UMuzzle*> UMuzzlesController::GetAvailableMuzzles()
{
	if (Muzzles.Num() == 0)
		return TArray<UMuzzle*>();

	UE_LOG(LogTemp, Warning, TEXT("Muzzles exist"));
	TArray<UMuzzle*> Output;

	for (UMuzzle* Muzzle : Muzzles)
		if (Muzzle->IsReadyToShoot())
			Output.Add(Muzzle);
	
	return Output;
}

ABaseProjectile*  UMuzzlesController::ShootWithMuzzle(int32 MuzzleId, const TSubclassOf<ABaseProjectile> ProjectileClass, const float ProjectileSpeed, const AActor* ProjectileTargetActor,const float Damage, const float Destruction, const float KnockbackIntensity)
{
	
	for (UMuzzle* Muzzle : Muzzles)
	{
		if (MuzzleId == Muzzle->GetMuzzleID())
		{
			return Muzzle->Shoot(ProjectileClass, ProjectileSpeed, ProjectileTargetActor, Damage, Destruction, KnockbackIntensity);
		}
	}
	return nullptr;
}

void UMuzzlesController::ShootWithMuzzles(const TArray<int32>& MuzzleIds, const TSubclassOf<ABaseProjectile> ProjectileClass, const float ProjectileSpeed, const AActor* ProjectileTargetActor, const float Damage, const float Destruction, const TArray<FMuzzleRandomAngle>& RandomAngles, const float KnockbackIntensity)
{

	for (UMuzzle* Muzzle : Muzzles)
	{
		if (!Muzzle) continue;

		int32 CurrentMuzzleId = Muzzle->GetMuzzleID();

		if (MuzzleIds.Contains(CurrentMuzzleId))
		{
			float RandomAngle = FindRandomAngleForMuzzle(RandomAngles, CurrentMuzzleId);
			
			ABaseProjectile* FiredProjectile = Muzzle->Shoot(ProjectileClass, ProjectileSpeed, ProjectileTargetActor, Damage, Destruction, KnockbackIntensity,RandomAngle);
		}
	}
	
}

float  UMuzzlesController::FindRandomAngleForMuzzle(const TArray<FMuzzleRandomAngle>& RandomAngles, int32 MuzzleId) 
{
	for (const FMuzzleRandomAngle& AngleInfo : RandomAngles)
	{
		if (AngleInfo.MuzzleId == MuzzleId)
			return AngleInfo.RandomAngle;
	}
	return 0.0f; 
}