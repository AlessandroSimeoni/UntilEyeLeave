// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "UObject/Interface.h"
#include "EffectHandler.generated.h"

class UAbilitySystemComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEffectHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TBO_01_API IEffectHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My Interface")
	void NotifyEffect(UAbilitySystemComponent* ASC, FActiveGameplayEffectHandle ActiveEffectHandle);
};
