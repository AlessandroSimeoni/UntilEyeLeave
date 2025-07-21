// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ShootingAbility.generated.h"

class ABaseProjectile;
/**
 * 
 */
UCLASS()
class TBO_01_API UShootingAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting|Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Shooting|Projectile")
	float ProjectileSpeed;
};
