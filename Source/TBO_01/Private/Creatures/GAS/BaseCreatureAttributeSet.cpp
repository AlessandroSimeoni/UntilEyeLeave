// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures/GAS/BaseCreatureAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"

void UBaseCreatureAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		//change Speed
		if (Attribute == GetSpeedAttribute())
		{
			if (UCharacterMovementComponent * CharacterMovementComponent = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
			{
				CharacterMovementComponent->MaxWalkSpeed = NewValue;
				CharacterMovementComponent->MaxFlySpeed = NewValue;
			
			}
		}
		if (Attribute == GetRotationSpeedAttribute())
		{
			if (UCharacterMovementComponent * CharacterMovementComponent = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
			{
				CharacterMovementComponent->RotationRate.Yaw = NewValue;
			
			}
		}
		//Check if monster is dead
		if (Attribute == GetHealthAttribute())
		{
			if (GetHealth() <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Health is 0"));
			}
		}
	}
}
