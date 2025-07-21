// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrackingProjectile.h"
#include "Creatures/Projectiles/BallisticProjectile.h"
#include "BackstabCreatureProjectile.generated.h"

class AAOEArea;
/**
 * 
 */
UCLASS()
class TBO_01_API ABackstabCreatureProjectile : public ATrackingProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AOE Spawn")
	FVector AOELineStartOffset = FVector::Zero();
	UPROPERTY(EditDefaultsOnly, Category = "AOE Spawn", Meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float AOELineTraceDistance = 1000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "AOE Spawn")
	TSubclassOf<AAOEArea> AOEAreaClass;
	FHitResult AOEHit;
	
	virtual void HandleCollision() override;
};
