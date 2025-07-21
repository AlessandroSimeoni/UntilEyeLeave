// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "TrackingProjectile.generated.h"

UCLASS()
class TBO_01_API ATrackingProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	
	virtual void Shoot(const UMuzzle* InstigatorMuzzle, float ProjectileSpeed, const AActor* ProjectileTargetActor) override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TWeakObjectPtr<AActor> Target;
	FVector CurrentVelocity;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true))
	float StraightTrajectoryDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true))
	float InterpSpeed =5.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile", meta=(AllowPrivateAccess=true))
	float GravityScale = 0.0f; 
	

	float ElapsedTime = 0.0f;

};
