// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/Action/DamageOverTimeAbility.h"

#include "AbilitySystemComponent.h"
#include "Interface/EffectHandler.h"

void UDamageOverTimeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	if (!TriggerEventData->Target || !DOTEffectClass)
		return;


	FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddInstigator(const_cast<AActor*>(TriggerEventData->Target.Get()), const_cast<AActor*>(TriggerEventData->Instigator.Get()));
	
	FGameplayEffectSpecHandle EffectSpec = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(DOTEffectClass, 1.0f, EffectContext);
	
	if (EffectSpec.IsValid())
	{
		FActiveGameplayEffectHandle ActiveEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

		if (TriggerEventData->Instigator->Implements<UEffectHandler>())
		{
			TScriptInterface<IEffectHandler> EffectHandlerInstance = const_cast<AActor*>(TriggerEventData->Instigator.Get());
			EffectHandlerInstance->Execute_NotifyEffect(const_cast<AActor*>(TriggerEventData->Instigator.Get()), ActorInfo->AbilitySystemComponent.Get(), ActiveEffectHandle);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("DOT Effect applied to %s"), *TriggerEventData->Target->GetName());
	}
}
