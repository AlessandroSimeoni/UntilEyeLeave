// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "Components/ActorComponent.h"
#include "MuzzlesController.generated.h"


class UMuzzle;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TBO_01_API UMuzzlesController : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TArray<UMuzzle*> Muzzles;

public:
	UFUNCTION(BlueprintCallable)
	TArray<UMuzzle*> GetAvailableMuzzles();

	UFUNCTION(BlueprintCallable)
	ABaseProjectile*  ShootWithMuzzle(int32 MuzzleId, TSubclassOf<ABaseProjectile> ProjectileClass, float ProjectileSpeed, const AActor*
	                     ProjectileTargetActor, float Damage, float Destruction, const float KnockbackIntensity);

	UFUNCTION(BlueprintCallable)
	void ShootWithMuzzles(const TArray<int32>& MuzzleIds, const TSubclassOf<ABaseProjectile> ProjectileClass,
		const float ProjectileSpeed, const AActor* ProjectileTargetActor, const float Damage, const float Destruction,const TArray<FMuzzleRandomAngle>& RandomAngles, const float KnockbackIntensity = 1.0f);

	static float FindRandomAngleForMuzzle(const TArray<FMuzzleRandomAngle>& RandomAngles, int32 MuzzleId);

};
