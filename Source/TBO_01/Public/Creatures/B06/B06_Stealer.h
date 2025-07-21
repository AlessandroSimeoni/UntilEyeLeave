// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures/Creature.h"
#include "B06_Stealer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayEffectAppliedSignature, FString, EffectName);

UCLASS()
class TBO_01_API AB06_Stealer : public ACreature
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Gameplay Effects")
	FOnGameplayEffectAppliedSignature OnGameplayEffectAppliedDelegate;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGameplayEffectApplied(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

};