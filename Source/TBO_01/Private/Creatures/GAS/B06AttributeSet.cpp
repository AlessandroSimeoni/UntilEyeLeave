// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/GAS/B06AttributeSet.h"

#include "Creatures/Creature.h"
#include "Currency/CoinContainer.h"

void UB06AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		if (Attribute == GetCoinAttribute())
		{
			ACreature* B06 = Cast<ACreature>(ActorInfo->AvatarActor);
			B06->CoinContainer->SetCoin(NewValue);
		}
	}
}
