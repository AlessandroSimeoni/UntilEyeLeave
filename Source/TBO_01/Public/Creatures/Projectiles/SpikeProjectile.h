// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Projectiles/StraightProjectile.h"
#include "SpikeProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReadyToShoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadOver);

/**
 * 
 */
UCLASS()
class TBO_01_API ASpikeProjectile : public AStraightProjectile
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "SpikeProjectile")
	void BroadcastLoadOver();
	UFUNCTION(BlueprintCallable, Category = "SpikeProjectile")
	void BroadcastReadyToShoot();
	virtual void HandleCollision() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FReadyToShoot OnReadyToShoot;
	UPROPERTY(BlueprintAssignable)
	FLoadOver OnLoadOver;
	virtual void BeginPlay() override;
	void Shoot(const UMuzzle* InstigatorMuzzle, const float ProjectileSpeed, const AActor* ProjectileTargetActor = nullptr) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SpikeProjectile")
	void LoadProjectile(float LoadTime, FVector TargetPosition);
	UFUNCTION(BlueprintImplementableEvent, Category = "SpikeProjectile")
	void StartShoot();

	
	float RandomAngleShoot = 0.0f;
};
