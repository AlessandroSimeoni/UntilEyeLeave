// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreatureAttributeSet.h"
#include "ShooterCreatureAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UShooterCreatureAttributeSet : public UBaseCreatureAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData BulletSpeed;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,BulletSpeed);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootChargeTime;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,ShootChargeTime);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData NonFocusDelayAfterCharge;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,NonFocusDelayAfterCharge);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootRecoilStrength;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,ShootRecoilStrength);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootRecoilTime;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,ShootRecoilTime);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootDelayTime;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,ShootDelayTime);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData AoE_Duration;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,AoE_Duration);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData AoE_Range;
	ATTRIBUTE_ACCESSORS(UShooterCreatureAttributeSet,AoE_Range);
};
