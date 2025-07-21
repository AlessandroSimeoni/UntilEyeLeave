// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreatureAttributeSet.h"
#include "B06AttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UB06AttributeSet : public UBaseCreatureAttributeSet
{
	GENERATED_BODY()

public:
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData Coin;
	ATTRIBUTE_ACCESSORS(UB06AttributeSet,Coin)

	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData RageDuration;
	ATTRIBUTE_ACCESSORS(UB06AttributeSet,RageDuration)

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ShieldedDuration;
	ATTRIBUTE_ACCESSORS(UB06AttributeSet,ShieldedDuration)

	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData AttackDelay;
	ATTRIBUTE_ACCESSORS(UB06AttributeSet,AttackDelay)

	
};
