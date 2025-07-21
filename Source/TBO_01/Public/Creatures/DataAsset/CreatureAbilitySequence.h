// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CreatureAbilitySequence.generated.h"

/**
 * 
 */
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityDelay
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	TSubclassOf<UGameplayAbility> Ability;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	float Delay;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	bool Event;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	FGameplayTag EventTag;
	
	FAbilityDelay()
	: Ability(nullptr)
	, Delay(0.0f)
	, Event(false)
	{}
};

				 
UCLASS(BlueprintType)
class TBO_01_API UCreatureAbilitySequence : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Abilities")
	TArray<FAbilityDelay> AbilityDelays;
};
