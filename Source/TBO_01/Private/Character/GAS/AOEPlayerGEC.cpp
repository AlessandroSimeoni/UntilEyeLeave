// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GAS/AOEPlayerGEC.h"

#include "AbilitySystemComponent.h"
#include "Character/MainCharacter.h"
#include "Creatures/Projectiles/AOEArea.h"

void UAOEPlayerGEC::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!TargetASC)
	{
		UE_LOG(LogTemp, Error, TEXT("Target ASC not found!"));
		return;
	}

	const AActor* TargetActor = TargetASC->GetAvatarActor();
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Target Actor not found!"));
		return;
	}

	FGameplayEffectContextHandle EffectContext = ExecutionParams.GetOwningSpec().GetContext();
	AActor* EffectCauser = EffectContext.GetEffectCauser();
	const AAOEArea* AOEArea = Cast<AAOEArea>(EffectCauser);
	if (!AOEArea)
	{
		UE_LOG(LogTemp, Error, TEXT("AOEArea Actor not found!"));
		return;
	}
	
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(const_cast<AActor*>(TargetActor));
	if (MainCharacter)
	{
		MainCharacter->DOTDamage(AOEArea->AOEDamage);
	}
}
