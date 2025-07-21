// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/GAS/BaseCreatureAttributeSet.h"
#include "B02AttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UB02AttributeSet : public UBaseCreatureAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData AttackingSpeed;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,AttackingSpeed);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData AttackDuration;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,AttackDuration);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData StunCooldown;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,StunCooldown);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData AttackChargeTime;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,AttackChargeTime);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData NonFocusDelayAfterCharge;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,NonFocusDelayAfterCharge);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData TimerExplosionDuration;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,TimerExplosionDuration);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ReducedTimerExplosionDuration;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,ReducedTimerExplosionDuration);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ExplosionDelay;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,ExplosionDelay);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ExplosionRadius;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,ExplosionRadius);
	
	FGameplayAttributeData KnockBackToTarget;
	ATTRIBUTE_ACCESSORS(UB02AttributeSet,KnockBackToTarget);
	
};
