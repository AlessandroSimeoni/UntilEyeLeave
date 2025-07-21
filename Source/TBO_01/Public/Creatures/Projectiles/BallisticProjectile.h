// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "GameFramework/Actor.h"
#include "BallisticProjectile.generated.h"

UCLASS()
class TBO_01_API ABallisticProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:	
	virtual void Shoot(const UMuzzle* InstigatorMuzzle, const float ProjectileSpeed, const AActor* ProjectileTargetActor) override;

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool FindProjectileDirectionNoGravity();

	

	/**0 is prefect prediction 1 is position at shoot*/
	UPROPERTY(EditAnywhere, Category = "Ballistic", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PredictionImprecision = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Settings")
	float MaxAngle = 45.0f;
	UPROPERTY()
	TObjectPtr<const AActor> TargetActor = nullptr;

	float Gravity = 0.0f;
	FVector ProjectileVelocity;;
	FVector ResultLocation;
};
