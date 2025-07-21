// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/B06/B06_Stealer.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"





void AB06_Stealer::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent = GetAbilitySystemComponent();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
			this,
			&AB06_Stealer::OnGameplayEffectApplied
		);
	}
}

void AB06_Stealer::OnGameplayEffectApplied(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	if (SpecApplied.Def)
	{
		const FString EffectName = SpecApplied.Def->GetName();
		OnGameplayEffectAppliedDelegate.Broadcast(EffectName);
	}
}