// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreatureAttributeSet.h"
#include "B05HeadAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UB05HeadAttributeSet : public UBaseCreatureAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData PhaseTransitionTime;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,PhaseTransitionTime);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData IntroEndWaitTime;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,IntroEndWaitTime);
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData EyeKnockback;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,EyeKnockback);
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootIntervalPhase1;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,ShootIntervalPhase1);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData SequenceIntervalPhase1;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,SequenceIntervalPhase1);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootSpeedSpikePhase1;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,ShootSpeedSpikePhase1);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootSpeedEyePhase1;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,ShootSpeedEyePhase1);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootIntervalPhase3;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,ShootIntervalPhase3);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData SequenceIntervalPhase3;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,SequenceIntervalPhase3);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootSpeedSpikePhase3;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,ShootSpeedSpikePhase3);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShootSpeedEyePhase3;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,ShootSpeedEyePhase3);

	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData BurstShootInterval;
	ATTRIBUTE_ACCESSORS(UB05HeadAttributeSet,BurstShootInterval);

	
};
