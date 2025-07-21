// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AOEDamage.h"
#include "Creatures/Projectiles/StraightTargetProjectile.h"
#include "StraightProjectileExplosive.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API AStraightProjectileExplosive : public AStraightTargetProjectile
{
	GENERATED_BODY()
	virtual void HandleCollision() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AOE")
	TSubclassOf<AAOEDamage> AOEExplosion;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AOE")
	float Size;
};
