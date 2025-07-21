// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCreatureAttributeSet.h"
#include "ShooterCreatureAttributeSet.h"
#include "B01DestructibleAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TBO_01_API UB01DestructibleAttributeSet : public UShooterCreatureAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData BrokenWalkSpeed;
	ATTRIBUTE_ACCESSORS(UB01DestructibleAttributeSet,BrokenWalkSpeed);
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FGameplayAttributeData BrokenWalkRotationSpeed;
	ATTRIBUTE_ACCESSORS(UB01DestructibleAttributeSet,BrokenWalkRotationSpeed);
	
};
