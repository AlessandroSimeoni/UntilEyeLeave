// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DamageOverTimeAbility.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UDamageOverTimeAbility : public UGameplayAbility
{
	GENERATED_BODY()


protected:
	virtual void ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Property")
	TSubclassOf<UGameplayEffect> DOTEffectClass;
};
