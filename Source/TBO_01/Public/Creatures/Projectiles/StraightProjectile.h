// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Projectiles/BaseProjectile.h"
#include "StraightProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API AStraightProjectile : public ABaseProjectile
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;
	
public:
	virtual void Shoot(const UMuzzle* InstigatorMuzzle, const float ProjectileSpeed, const AActor* ProjectileTargetActor) override;
};
