// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Projectiles/Muzzle.h"
#include "SpikeMuzzle.generated.h"

class ASpikeProjectile;
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class TBO_01_API USpikeMuzzle : public UMuzzle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASpikeProjectile> SpikeProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector SpawnOffsetFromMuzzle = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly)
	ASpikeProjectile* SpikeProjectileInstance = nullptr;
	float Speed = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "SpikeMuzzle")
	void PrepareNextShot();
	UFUNCTION(BlueprintCallable, Category = "SpikeMuzzle")
	void ShootSpike();
	UFUNCTION(Category = "SpikeMuzzle")
	void HandleSpikeReady();

	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bPromoteChildren = false) override;
	
	
public:
	virtual ABaseProjectile*  Shoot(const TSubclassOf<ABaseProjectile> ProjectileClass, const float ProjectileSpeed, const AActor* ProjectileTargetActor,const float Damage = 0.5f, const float Destruction = 0,  const float KnockbackIntensity = 1.f, const float RandomAngle = 0.0f) override;
};
