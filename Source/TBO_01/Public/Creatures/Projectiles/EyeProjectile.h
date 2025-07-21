#pragma once

#include "CoreMinimal.h"
#include "TrackingProjectile.h"
#include "EyeProjectile.generated.h"

class UEyeSceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEyeProjectileDestroyed);

UCLASS()
class TBO_01_API AEyeProjectile : public ATrackingProjectile
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="EyeProjectile")
	FOnEyeProjectileDestroyed OnEyeProjectileDestroyed;

	AEyeProjectile();
	virtual void Shoot(const UMuzzle* InstigatorMuzzle, float ProjectileSpeed, const AActor* ProjectileTargetActor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="Eye")
	TObjectPtr<UEyeSceneComponent> EyeSceneComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FEffectData DestructionFX;

	UFUNCTION()
	void ProjectileDestroyed(UEyeSceneComponent* This, const AActor* Destroyer);
};
