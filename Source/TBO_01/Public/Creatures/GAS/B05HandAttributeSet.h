// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreatureAttributeSet.h"
#include "B05HandAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UB05HandAttributeSet : public UBaseCreatureAttributeSet
{
public:
	
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData RingMaxExpansion;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,RingMaxExpansion);
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData RingExpansionTime;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,RingExpansionTime);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData RingDamage;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,RingDamage);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData RingKnockBack;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,RingKnockBack);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData StompSequenceNumber;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,StompSequenceNumber);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData StompHeight;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,StompHeight);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData LastStompDownDuration;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,LastStompDownDuration);

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData DelayBetweenSequence;
	ATTRIBUTE_ACCESSORS(UB05HandAttributeSet,DelayBetweenSequence);
};
