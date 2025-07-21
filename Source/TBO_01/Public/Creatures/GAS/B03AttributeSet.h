// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreatureAttributeSet.h"
#include "ShooterCreatureAttributeSet.h"
#include "B03AttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UB03AttributeSet : public UShooterCreatureAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData ChasingSpeed;
	ATTRIBUTE_ACCESSORS(UB03AttributeSet,ChasingSpeed);
	
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData SearchingTime;
	ATTRIBUTE_ACCESSORS(UB03AttributeSet,SearchingTime)

	
};
